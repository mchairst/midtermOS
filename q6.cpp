#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <limits>

using namespace std;

// Helper to split a command line string into arguments for execvp
vector<char*> parse_command(const string& input) {
    vector<char*> args;
    stringstream ss(input);
    string token;
    while (ss >> token) {
        char* arg = new char[token.size() + 1];
        strcpy(arg, token.c_str());
        args.push_back(arg);
    }
    args.push_back(nullptr); // execvp needs null-terminated array
    return args;
}

void free_args(vector<char*>& args) {
    for (char* arg : args) {
        delete[] arg;
    }
}

int main() {
    string command;

    while (true) {
        cout << "closh> ";
        getline(cin, command);

        if (command.empty()) continue;
        if (command == "exit") break;

        // Ask for count
        cout << "count> ";
        int count;
        cin >> count;
        if (count < 1 || count > 9) {
            cerr << "Please enter a number between 1 and 9.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        // Ask for execution mode
        cout << "[p]arallel or [s]equential> ";
        char mode;
        cin >> mode;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear newline

        // Parse the command into args for execvp
        vector<char*> args = parse_command(command);
        if (args.empty()) continue;

        vector<pid_t> pids;

        for (int i = 0; i < count; ++i) {
            pid_t pid = fork();
            if (pid < 0) {
                perror("fork");
                free_args(args);
                exit(EXIT_FAILURE);
            } else if (pid == 0) {
                // Child process
                execvp(args[0], args.data());
                perror("execvp"); // Only runs if execvp fails
                exit(EXIT_FAILURE);
            } else {
                // Parent
                if (mode == 's' || mode == 'S') {
                    waitpid(pid, nullptr, 0); // Sequential wait
                } else if (mode == 'p' || mode == 'P') {
                    pids.push_back(pid); // Save PID for later wait
                } else {
                    cerr << "Invalid mode. Use 'p' or 's'.\n";
                    break;
                }
            }
        }

        // Wait for all children in parallel mode
        if (mode == 'p' || mode == 'P') {
            for (pid_t pid : pids) {
                waitpid(pid, nullptr, 0);
            }
        }

        free_args(args);
    }

    return 0;
}
