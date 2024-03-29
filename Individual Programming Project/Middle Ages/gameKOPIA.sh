#!/bin/bash

#Checks if given parameter n is compliant with the rules.
checkNRange() {
	if [ $1 -lt 9 ] || [ $1 -gt 2147483648 ]; then
		exit 1
	fi
}

#Checks if given parameter k is compliant with the rules.
checkKRange() {
	if [ $1 -lt 1 ] || [ $1 -gt 2147483648 ]; then
		exit 1
	fi
}

#Returns greater of two arguments.
max() {
	if [ $# != 2 ]; then
		exit 1
	fi
	
	if [ $1 -gt $2 ]; then
		echo $1
	else
		echo $2
	fi
}

#Returns absolute value of difference of arguments.
absDiff() {
	if [ $# != 2 ]; then
		exit 1
	fi
	
	difference=$(( $1 - $2 ))
	
	if [ $difference -lt 0 ]; then
		difference=$(( -$difference ))
	fi
	
	echo $difference
}

#Name of function speaks for itself.
#arguments: (1.n, 2.x, 3.y)
checkIfThereIsSpaceForSecondKing() {
	if [ $# != 3 ]; then
		exit 1
	fi
	
	fromLeftToKing=$(absDiff 0 $2)
	fromKingToRight=$(absDiff $2 $1)
	let "fromKingToRight -= 3" #squares for knights and peasant
	fromUpToKing=$(absDiff 0 $3)
	fromKingToBottom=$(absDiff $3 $1)
	
	if [ $(max $fromLeftToKing $fromKingToRight) -lt 8 ] && [ $(max $fromUpToKing $fromKingToBottom) -lt 8 ]; then
		echo 1
	else 
		echo 0
	fi
}

#Returns distance between kings in metric maximum.
#arguments: (1.x1, 2.x2, 3.y1, 4.y2)
kingsDistance() {
	echo $(max $(absDiff $1  $2) $(absDiff $3  $4))
}

#Exits script, if given argument is not a number (using regex comparison)
checkIfItIsNumber() {
	reg='^[0-9]+$'
	if ! [[ $1 =~ $reg ]] ; then
		echo "error: Not a number" >&2
		exit 1
	fi
}

#Creates in and out pipes for process and attaches them to file descriptiors
#arguments: ( $1, $2 <- file descriptors' numbers )
init_pipes() {
	#in
	pipe=$(mktemp -u)
	mkfifo $pipe
	eval "exec $1<>$pipe"
	rm $pipe

	#out
	pipe=$(mktemp -u)
	mkfifo $pipe
	eval "exec $2<>$pipe"
	rm $pipe
}

#Prints all controlled parameters; dev usage
print_params() {
	echo "n = $n"
	echo "k = $k"
	echo "s = $s"
	echo "x1 = $x1"
	echo "y1 = $y1"
	echo "x2 = $x2"
	echo "y2 = $y2"
	echo "ai1 = $ai1"
	echo "ai2 = $ai2"
}

#Checks is process exists.
#arguments: (1.pid_to_check)
break_if_process_ended() {
	if ! kill -0 $1; then
		break
	fi
}

#Reads line and echoes it from/to given file descriptors.
# $1 - number of file descriptor to read line from, 
# $2, $3 - numbers of file descriptors to echo the line.
# $4 - number of seconds to wait
read_line_and_echo() {
	read line <&$1
	echo $line >&$2
	if [ $# -gt 2 ]; then
		echo $line >&$3
		sleep $4
	fi
}

#Closes all file descriptors.
close_file_descriptors() {
	exec 3>&-
	exec 4>&-
	exec 5>&- 
	exec 6>&- 
	exec 7>&- 
	exec 8>&- 
}

#Kill all child processess.
killtree() {
    local _pid=$1
    local _sig=${2:--TERM}
    kill -stop ${_pid} # needed to stop quickly forking parent from producing 
children between child killing and parent killing
    for _child in $(ps -o pid --no-headers --ppid ${_pid}); do
        killtree ${_child} ${_sig}
    done
    kill -${_sig} ${_pid}
}

#Step 0: 
#declaring default values of game parameters (0 = not given by user).
n=10
k=100
s=1
x1=0
y1=0
x2=0
y2=0
ai1=0
ai2=0
ai1_is_still_alive=0 # 0 means true
ai2_is_still_alive=0
error=0

kluski=0
i=0
pipe=dudapa
mkfifo $pipe
exec 9<>$pipe
rm $pipe


#Step I: 
#reading all arguments, checking if they are numbers less than INTMAX etc. Only format of arguments (not values) is checked here.
i=1
while [ "$i" -le $# ]; do
	if [ "$1" == "-n" ]; then
		shift
		n=$1
		checkIfItIsNumber $n
		checkNRange $n
	elif [ "$1" == "-k" ]; then
		shift
		k=$1
		checkIfItIsNumber $k
		checkKRange $k
	elif [ "$1" == "-s" ]; then
		shift
		s=$1
		checkIfItIsNumber $s
	elif [ "$1" == "-p1" ]; then
		shift
		str=$1
		if ! [[ $str =~ .*,.* ]]; then
			exit 1
		fi
		x1=${str%,*}
		y1=${str#*,}
		checkIfItIsNumber $x1
		checkIfItIsNumber $y1
	elif [ "$1" == "-p2" ]; then
		shift
		str=$1
		if ! [[ $str =~ .*,.* ]]; then
			exit 1
		fi
		x2=${str%,*}
		y2=${str#*,}
		checkIfItIsNumber $x2
		checkIfItIsNumber $y2
	elif [ "$1" == "-ai1" ]; then
		shift
		ai1=$1
		#cannot check if it's reasonable argument in this script
	elif [ "$1" == "-ai2" ]; then
		shift
		ai2=$1
		#cannot check if it's reasonable argument in this script
	else
		exit 1
	fi

	shift
done

#Step II: 
#If both kings are given, checks if king's positions are in board or if they aren't to close.
#If only one king is given, checks if it's possible to draw position of another one and does it.
#If none of kings is given, draws their positions (according to game rules).             
if [ $x1 -eq 0 ]; then
	#first king's position needs to be drawn
	if [ $x2 -eq 0 ]; then 
		#both kings' positions need to be drawn
		while [ $x1 -lt 1 ] || [ $(kingsDistance $x1 $x2 $y1 $y2) -lt 8 ]; do
			
			x1=$(shuf -i 1-$(( n - 4 )) -n1)
			y1=$(shuf -i 1-$n -n1)
			x2=$(shuf -i 1-$(( n - 4 )) -n1)
			y2=$(shuf -i 1-$n -n1)
			
		done
	elif [ $x2 -gt 0 ] && [ $y2 -gt 0 ] && [ $x2 -le $(( n - 4 )) ] && \
	     [ $y2 -le $n ] && \
	     [ $(checkIfThereIsSpaceForSecondKing $n $x2 $y2) -eq 0 ]; then
		#second king's position is given and parameters are ok
		while [ $x1 -lt 1 ] || [ $(kingsDistance $x1 $x2 $y1 $y2) -lt 8 ]; do
			
			x1=$(shuf -i 1-$(( n - 4 )) -n1)
			y1=$(shuf -i 1-$n -n1)
			
		done
	else
		#second king's position is unable to be properly drawn or given parameters are invalid
		exit 1
	fi
elif [ $x2 -eq 0 ]; then
	#second king's position needs to be drawn, first king's position is given - I need to check if it's valid
	if [ $x1 -gt 0 ] && [ $y1 -gt 0 ] && [ $x1 -lt $n ] && [ $y1 -lt $n ] && [ $(checkIfThereIsSpaceForSecondKing $n $x1 $y1) -eq 0 ]; then
		#first king's position is given and parameters are ok
		while [ $x2 -lt 1 ] || [ $(kingsDistance $x1 $x2 $y1 $y2) -lt 8 ]; do
			
			x2=$(shuf -i 1-$(( n - 4 )) -n1)
			y2=$(shuf -i 1-$n -n1)
			
		done
	else
		#first king's position is unable to be properly drawn or given parameters are invalid
		exit 1
	fi
else
	#both kings' positions are given; it only has to be checked if given parameters are valid
	if [ $x1 -le 0 ] || [ $y1 -le 0 ] || [ $x2 -le 0 ] || [ $y2 -le 0 ] || [ $x1 -gt $n ] || [ $y1 -gt $n ] || [ $x2 -gt $n ] || [ $y2 -gt $n ] || [ $(kingsDistance $x1 $x2 $y1 $y2) -lt 8 ];then
		exit 1
	fi
fi

print_params $n $k $s $x1 $y2 $x2 $y2 $ai1 $ai2


#Step III:
#Play the game in one of three modes: ai vs ai, ai vs human or human vs human

#gui in & out
init_pipes 3 4

if [ $ai1 != "0" ]; then
#using ai as first player

	#ai1 in & out
	init_pipes 5 6
	
	if [ $ai2 != "0" ]; then 
	#ai1 vs ai2
		#ai2 in & out
		init_pipes 7 8

		./sredniowiecze_gui_with_libs.sh <&3 >&4 &
		pid_gui=$!
		#trap "kill $pid_gui 2> /dev/null" EXIT

		$ai1 <&5 >&6 &
		pid_ai1=$!
		
		$ai2 <&7 >&8 &
		pid_ai2=$!		

		echo "INIT $n $k 1 $x1 $y1 $x2 $y2" >&3
		echo "INIT $n $k 2 $x1 $y1 $x2 $y2" >&3
		
		echo "INIT $n $k 1 $x1 $y1 $x2 $y2" >&5
		echo "INIT $n $k 2 $x1 $y1 $x2 $y2" >&7
		
		tury=$k
		while kill -0 $pid_gui >/dev/null 2>&1; do
			
			if [ $ai1_is_still_alive -eq 0 ]; then
				tury=$(( $tury - 1 ))
				if [ $tury -eq 0 ]; then
					echo "END_TURN" >&7
				else
					read_line_and_echo 6 3 7 $s
				fi
				
				if ! kill -0 $pid_ai1 >/dev/null 2>&1; then
					ai1_is_still_alive=1;
					ai1_return_code=$?
				fi
				
				while [ "$line" != "END_TURN" ] && \
			              [ $ai1_is_still_alive -eq 0 ]; do
					read_line_and_echo 6 3 7 $s
					if [ ! kill -0 $pid_ai1 >/dev/null 2>&1 ]; then
						ai1_is_still_alive=1;
						ai1_return_code=$?;
						break;
					fi
					#################DODANE
					if [ ! kill -0 $pid_ai2 >/dev/null 2>&1 ]; then
						ai2_is_still_alive=1;
						ai2_return_code=$?;
						break;
					fi
					####################
				done
				
			fi
				
			if [ $ai1_is_still_alive -eq 1 ]; then
				if ! { [ $ai1_return_code -eq 0 ] || \
				   [ $ai1_return_code -eq 1 ] || \
				   [ $ai1_return_code -eq 2 ]; }; then
					error=1
					break
				fi
			fi
			
			
			if [ $ai2_is_still_alive -eq 0 ]; then
			
				read_line_and_echo 8 3 5 $s
				
				if  ! kill -0 $pid_ai2 >/dev/null 2>&1; then
					ai2_is_still_alive=1;
					ai2_return_code=$?
				fi
				
				while [ "$line" != "END_TURN" ] && \
			              [ $ai2_is_still_alive -eq 0 ]; do
					read_line_and_echo 8 3 5 $s
					if [ ! kill -0 $pid_ai2 >/dev/null 2>&1 ]; then
						ai2_is_still_alive=1;
						ai2_return_code=$?;
						break;
					fi
					####DODANE
					if [ ! kill -0 $pid_ai1 >/dev/null 2>&1 ]; then
						ai1_is_still_alive=1;
						ai1_return_code=$?;
						break;
					fi
					######
				done
				
			fi
			
			if [ $ai2_is_still_alive -eq 1 ]; then
				if ! { [ $ai2_return_code -eq 0 ] || \
				   [ $ai2_return_code -eq 1 ] || \
				   [ $ai2_return_code -eq 2 ]; }; then
					error=1
					break
				fi
			fi
			
			
			if [ $ai1_is_still_alive -eq 1 ] && \
			   [ $ai2_is_still_alive -eq 1 ]; then
				break
			fi
			
		done
		
		
		if [ $error -eq 1 ]; then
			exit 1
		fi
		
		wait $pid_gui
		
		if [ ! $? -eq 0 ]; then
			exit 1
		fi
		
		close_file_descriptors
		#killtree $@
		exit 0
		
		
	else
	#ai1 vs human2
		./sredniowiecze_gui_with_libs.sh -human2 <&3 >&4 &
		pid_gui=$!

		$ai1 <&5 >&6 &
		pid_ai1=$!
		
		echo "INIT $n $k 1 $x1 $y1 $x2 $y2" >&3
		echo "INIT $n $k 2 $x1 $y1 $x2 $y2" >&3
		
		echo "INIT $n $k 1 $x1 $y1 $x2 $y2" >&5
		
		tury=$k
		while kill -0 $pid_gui >/dev/null 2>&1; do
			
			echo "bede czytal z ai"
			#########################
			#reading from ai, writing to gui
			if ! kill -0 $pid_ai1 >/dev/null 2>&1; then
				echo "ai1 zmarlo smiercia naturalna z kodem $? w pierwszym"
				ai1_return_code=$?
				break
			fi
			if ! kill -0 $pid_gui >/dev/null 2>&1; then
				gui_return_code=$?
				break
			fi

			read_line_and_echo 6 3
			echo "odczytalem"
			if ! kill -0 $pid_ai1 >/dev/null 2>&1; then
				echo "ai1 zmarlo smiercia naturalna zkodem $? w drugim ifie"
				ai1_return_code=$?
				break
			fi
			if ! kill -0 $pid_gui >/dev/null 2>&1; then
				gui_return_code=$?
				break
			fi
			
			while [ "$line" != "END_TURN" ]; do
				read_line_and_echo 6 3
				if ! kill -0 $pid_ai1 >/dev/null 2>&1; then
					echo "ai1 zmarlo smiercia naturalna zkodem $? w trzecim ifie"

					break
				fi
				if ! kill -0 $pid_gui >/dev/null 2>&1; then
				gui_return_code=$?
				break
			fi
			done
						
			if ! kill -0 $pid_ai1 >/dev/null 2>&1; then
				ai1_return_code=$?
				echo "ai1 zmarlo smiercia naturalna zkodem $? w czwartym ifie"

				break
			fi
			if ! kill -0 $pid_gui >/dev/null 2>&1; then
				gui_return_code=$?
				break
			fi
			##########################
			
			##########################
			#reading from gui, writing to ai

			read_line_and_echo 4 5
			if ! kill -0 $pid_gui >/dev/null 2>&1; then
				gui_return_code=$?
				break
			fi
			if ! kill -0 $pid_ai1 >/dev/null 2>&1; then
				echo "ai1 zmarlo smiercia naturalna z kodem $? w piatym ifie"
				ai1_return_code=$?
				break
			fi
			echo "siema"
			while [ "$line" != "END_TURN" ]; do
				read_line_and_echo 4 5
				if ! kill -0 $pid_gui >/dev/null 2>&1; then
					break
				fi
				if ! kill -0 $pid_ai1 >/dev/null 2>&1; then
					echo "ai1 zmarlo smiercia naturalna z kodem $? w szostym ifie"
					ai1_return_code=$?
				break
			fi
			done
			echo "no elo"
			if ! kill -0 $pid_gui >/dev/null 2>&1; then
				gui_return_code=$?
				break
			fi
			if ! kill -0 $pid_ai1 >/dev/null 2>&1; then
				echo "ai1 zmarlo smiercia naturalna z kodem $? w siodmym ifie"
				ai1_return_code=$?
				break
			fi
			
			############################
		done
				
		if ! kill -0 $pid_ai1 >/dev/null 2>&1; then
			if ! { [ $ai1_return_code -eq 0 ] || \
				[ $ai1_return_code -eq 1 ] || \
				[ $ai1_return_code -eq 2 ]; }; then
				exit 1
			fi
		fi
		
		wait $pid_gui
		if [ ! $? -eq 0 ]; then
			exit 1
		fi		
		
		close_file_descriptors
		#killtree $@
		exit 0
		
	fi
	
else
#not using ai as a first player

	if [ $ai2 != "0" ]; then 
	#human1 vs ai2
	
		#ai2 in & out
		init_pipes 7 8

		./sredniowiecze_gui_with_libs.sh -human1 <&3 >&4 &
		pid_gui=$!

		$ai2 <&7 >&8 &
		pid_ai2=$!
	
		echo "INIT $n $k 1 $x1 $y1 $x2 $y2" >&3
		echo "INIT $n $k 2 $x1 $y1 $x2 $y2" >&3
		
		echo "INIT $n $k 2 $x1 $y1 $x2 $y2" >&7
		
		while kill -0 $pid_gui >/dev/null 2>&1; do
		
			#######################
			#Read from human(gui) and send to ai2
			read_line_and_echo 4 7
			if ! kill -0 $pid_gui >/dev/null 2>&1; then
				gui_return_code=$?
				break
			fi
			while [ "$line" != "END_TURN" ]; do
				read_line_and_echo 4 7
				if ! kill -0 $pid_gui >/dev/null 2>&1; then
					break
				fi
			done
			if ! kill -0 $pid_gui >/dev/null 2>&1; then
				gui_return_code=$?
				break
			fi
			#####################
			########################
			#Read from ai2 and send to human(gui)
			if ! kill -0 $pid_ai2 >/dev/null 2>&1; then
				ai2_return_code=$?
			fi
			read_line_and_echo 8 3 
			if ! kill -0 $pid_ai2 >/dev/null 2>&1; then
				ai2_return_code=$?
				break
			fi
			while [ "$line" != "END_TURN" ]; do
				read_line_and_echo 8 3
				if ! kill -0 $pid_ai2 >/dev/null 2>&1; then
					break
				fi
			done
			if ! kill -0 $pid_ai2 >/dev/null 2>&1; then
				ai2_return_code=$?
				break
			fi
			#########################
		done
		if ! kill -0 $pid_ai2 >/dev/null 2>&1; then
			if ! { [ $ai2_return_code -eq 0 ] || \
				[ $ai2_return_code -eq 1 ] || \
				[ $ai2_return_code -eq 2 ]; }; then
				exit 1
			fi
		fi
		
		wait $pid_gui
		if [ ! $? -eq 0 ]; then
			exit 1
		fi		
		
		close_file_descriptors
		#killtree $@
		exit 0			
	
		
	else
	#human1 vs human2
				
		./sredniowiecze_gui_with_libs.sh -human1 -human2 <&3 >&4 &
		pid_gui=$!
		
		echo "INIT $n $k 1 $x1 $y1 $x2 $y2" >&3
		echo "INIT $n $k 2 $x1 $y1 $x2 $y2" >&3
		wait $pid_gui
		
		if [ ! $? -eq 0 ]; then
			exit 1
		else
			exit 0
		fi
		
	fi

fi
		


