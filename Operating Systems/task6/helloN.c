#include <minix/drivers.h>
#include <minix/chardriver.h>
#include <stdio.h>
#include <stdlib.h>
#include <minix/ds.h>
#include "helloN.h"

/*
 * Function prototypes for the hello driver.
 */
static int helloN_open(devminor_t minor, int access, endpoint_t user_endpt);
static int helloN_close(devminor_t minor);
static ssize_t helloN_read(devminor_t minor, u64_t position, endpoint_t endpt,
    cp_grant_id_t grant, size_t size, int flags, cdev_id_t id);
static ssize_t helloN_write(devminor_t minor, u64_t position, endpoint_t endpt,
    cp_grant_id_t grant, size_t size, int flags, cdev_id_t id);

/* SEF functions and variables. */
static void sef_local_startup(void);
static int sef_cb_init(int type, sef_init_info_t *info);
static int sef_cb_lu_state_save(int);
static int lu_state_restore(void);

/* Entry points to the hello driver. */
static struct chardriver helloN_tab =
{
    .cdr_open	= helloN_open,
    .cdr_close	= helloN_close,
    .cdr_read	= helloN_read,
    .cdr_write  = helloN_write,
};

/** State variable to count the number of times the device has been opened.
 * Note that this is not the regular type of open counter: it never decreases.
 */
static int open_counter;

static char buffer[DEVICE_SIZE];

static int helloN_open(devminor_t UNUSED(minor), int UNUSED(access),
    endpoint_t UNUSED(user_endpt))
{
    // printf("helloN_open(). Called %d time(s).\n", ++open_counter);
    return OK;
}

static int helloN_close(devminor_t UNUSED(minor))
{
    return OK;
}

static ssize_t helloN_read(devminor_t UNUSED(minor), u64_t position,
    endpoint_t endpt, cp_grant_id_t grant, size_t size, int UNUSED(flags),
    cdev_id_t UNUSED(id))
{
    char *ptr;
    int ret;

    /* Check for EOF, and possibly limit the read size. */
    if (position >= DEVICE_SIZE) return 0;		/* EOF */
    if (position + size > DEVICE_SIZE)
        size = (size_t)(DEVICE_SIZE - position);	/* limit size */

    /* Copy the requested part to the caller. */
    ptr = buffer + (size_t)position;
    if ((ret = sys_safecopyto(endpt, grant, 0, (vir_bytes) ptr, size)) != OK)
        return ret;

    /* Return the number of bytes read. */
    return size;
}

static ssize_t helloN_write(devminor_t UNUSED(minor), u64_t position,
    endpoint_t endpt, cp_grant_id_t grant, size_t size, int UNUSED(flags),
    cdev_id_t UNUSED(id))
{
    char *ptr;
    int ret;

    // printf("helloN_write()\n");

    /* Check for EOF, and possibly limit the read size. */
    if (position >= DEVICE_SIZE) return 0;      /* EOF */
    if (position + size > DEVICE_SIZE)
        size = (size_t)(DEVICE_SIZE - position);    /* limit size */

    /* Write bytes to buffer starting from position */
    ptr = buffer + (size_t)position;
    if ((ret = sys_safecopyfrom(endpt, grant, 0, (vir_bytes) ptr, size)) != OK)
        return ret;

    /* Return the number of bytes written. */
    return size;
}

static int sef_cb_lu_state_save(int UNUSED(state)) {
/* Save the state. */
    ds_publish_u32("open_counter", open_counter, DSF_OVERWRITE);
    ds_publish_str("buffer", buffer, DSF_OVERWRITE);

    return OK;
}

static int lu_state_restore() {
/* Restore the state. */
    u32_t value;

    ds_retrieve_u32("open_counter", &value);
    ds_delete_u32("open_counter");
    open_counter = (int) value;

    ds_retrieve_str("buffer", buffer, DEVICE_SIZE);
    ds_delete_str("buffer");

    return OK;
}

static void sef_local_startup()
{
    /*
     * Register init callbacks. Use the same function for all event types
     */
    sef_setcb_init_fresh(sef_cb_init);
    sef_setcb_init_lu(sef_cb_init);
    sef_setcb_init_restart(sef_cb_init);

    /*
     * Register live update callbacks.
     */
    /* - Agree to update immediately when LU is requested in a valid state. */
    sef_setcb_lu_prepare(sef_cb_lu_prepare_always_ready);
    /* - Support live update starting from any standard state. */
    sef_setcb_lu_state_isvalid(sef_cb_lu_state_isvalid_standard);
    /* - Register a custom routine to save the state. */
    sef_setcb_lu_state_save(sef_cb_lu_state_save);

    /* Let SEF perform startup. */
    sef_startup();
}

static int sef_cb_init(int type, sef_init_info_t *UNUSED(info))
{
/* Initialize the hello driver. */
    int do_announce_driver = TRUE;

    open_counter = 0;
    int i = 0;
    switch(type) {
        case SEF_INIT_FRESH:
            while(i < DEVICE_SIZE) {
                buffer[i++] = 'a';
            }
        break;

        case SEF_INIT_LU:
            /* Restore the state. */
            lu_state_restore();
            do_announce_driver = FALSE;

            // printf("%sHey, I'm a new version!\n", HELLO_MESSAGE);
        break;

        case SEF_INIT_RESTART:
            // printf("%sHey, I've just been restarted!\n", HELLO_MESSAGE);
        break;
    }

    /* Announce we are up when necessary. */
    if (do_announce_driver) {
        chardriver_announce();
    }

    /* Initialization completed successfully. */
    return OK;
}

int main(void)
{
    /*
     * Perform initialization.
     */
    sef_local_startup();

    /*
     * Run the main loop.
     */
    chardriver_task(&helloN_tab);
    return OK;
}
