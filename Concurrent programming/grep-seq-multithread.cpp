/**
 * @file grep-seq-multithread.cpp
 * Multithread version of grep finding word in given text files.
 *
 * Input:
 * word
 * number_of_files
 * file_name1
 * file_name2
 * ...
 *
 * Compilation:
 * g++ -c grep-seq-multithread.cpp -pthread -std=c++14
 * g++ grep-seq-multithread.o -o grep-seq-multithread -pthread -std=c++14
 *
 * Example text files in data-txt directory.
 *
 * @author Marcin Byra
 */

#include <iostream>
#include <fstream>
#include <locale>
#include <string>
#include <vector>
#include <codecvt>

#include <thread>
#include <future>

static int number_of_threads = 4;

int grep (std::string filename, std::wstring word) {
    std::locale loc("pl_PL.UTF-8");
    std::wfstream file(filename);
    file.imbue(loc);
    std::wstring line;
    unsigned int count = 0;
    while (getline(file, line)) {
        for (auto pos = line.find(word,0);
             pos != std::string::npos;
             pos = line.find(word, pos+1))
            count++;
    }
    return count;
}

void thread_grep(std::vector<std::string> filenames, std::wstring word, int a, int b, std::promise<int>& count_promise) {
    int count = 0;
    std::cout << "Starting new thread (a = " << a << ", b = " << b << ")" << std::endl;

    for (auto i = a; i != b; ++i) {
        count += grep(filenames[i], word);
    }
    count_promise.set_value(count);
    std::cout << "Thread (a = " << a << ", b = " << b << ") completed" << std::endl;

}

int main() {
    std::ios::sync_with_stdio(false);
    std::locale loc("pl_PL.UTF-8");
    std::wcout.imbue(loc);
    std::wcin.imbue(loc);

    std::wstring word;
    std::getline(std::wcin, word);

    std::wstring s_file_count;
    std::getline(std::wcin, s_file_count);
    int file_count = std::stoi(s_file_count);

    std::vector<std::string> filenames{};
    
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

    for (int file_num = 0; file_num < file_count; file_num++) {
        std::wstring w_filename;
        std::getline(std::wcin, w_filename);
        std::string s_filename = converter.to_bytes(w_filename);
        filenames.push_back(s_filename);
    }

    std::cout << std::endl << "Starting main(). Number of threads available: " << number_of_threads << std::endl;

    int count_sum = 0;
    std::vector<int> count (number_of_threads, 0);

    std::vector<std::promise<int>> count_promise (number_of_threads);
    std::vector<std::future<int>> count_future (number_of_threads);
    for (auto i = 0; i < number_of_threads; ++i) {
        count_future[i] = count_promise[i].get_future();
    }

    // divide with rounding up to avoid 0 when there are more threads than files:
    int files_for_one_thread = ( filenames.size() + number_of_threads - 1 ) / number_of_threads;
    int a = 0;
    int b = files_for_one_thread;
    int number_of_threads_used = 0;

    std::vector<std::thread> threads;
    for (auto i = 0; i < number_of_threads; ++i) {

        threads.push_back(std::thread {[=, &count_promise] { thread_grep(filenames, word, a, b, count_promise[i]); }});

        number_of_threads_used++;
        if (b >= filenames.size()) {
            break; // don't start next threads if all filenames are assigned
        }
        a = b;
        // last set of filenames may be shorter:
        b = a + files_for_one_thread < filenames.size() ? a + files_for_one_thread : filenames.size(); 
    }


    for (auto i = 0; i < number_of_threads_used; ++i) {
        count[i] = count_future[i].get();
        count_sum += count[i];
    }

    for (auto i = 0; i < number_of_threads_used; ++i) {
        threads[i].join();

    }

    std::cout << std::endl << count_sum << std::endl;
}