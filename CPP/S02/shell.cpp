extern "C" {
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <cerrno>
#include <fcntl.h>
#include "readcmd.h"
}

#include <iostream>

template<typename T>
class DynamicList {
protected:
    int _length = 0;
    int maxLength = 20;
    T *data;
public:
    explicit DynamicList(T *data) {
        this->data = (T *) calloc(maxLength, sizeof(T));
        for (int i = 0; data[i] != nullptr; i++) {
            this->push(data[i]);
        }
    }

    void push(T item) {
        if (this->_length >= this->maxLength) {
            this->maxLength *= 2;
            this->data = (T *) realloc(this->data, sizeof(T) * this->maxLength);
        }
        this->data[this->_length] = item;
        this->_length++;
    }

    T pop() {
        if (this->_length >= 0) {
            this->_length--;
            return this->data[this->_length];
        }
        std::throw_with_nested("Empty array.");
    }

    virtual int length() {
        return this->_length;
    }

    T get(int i) {
        if (i < 0 || i >= this->_length) {
            std::throw_with_nested("Out of bounds.");
        }
        return this->data[i];
    }

    DynamicList() {
        this->data = (T *) calloc(maxLength, sizeof(T));
    }
};

class PipeList: private DynamicList<int *> {
public:
    PipeList(int length) {
        for (int i = 0; i < length; i++) {
            int* p = (int *)calloc(2, sizeof(int));
            pipe(p);
            this->push(p);
        }
    }

    int getStdOut(int index) {
        return this->data[index][1];
    }

    void dup2StdOut(int index) {
        dup2(this->getStdOut(index), 1);
    }

    int getStdIn(int index) {
        return this->data[index][0];
    }

    void dup2StdIn(int index) {
        dup2(this->getStdIn(index), 0);
    }

    void closeAll() {
        for (int i = 0; i < this->_length; i++) {
            close(getStdIn(i));
            close(getStdOut(i));
        }
        free(this);
    }

    int length() override {
        return this->_length;
    }

    void display() {
        std::cout << "[" << std::endl;
        for (int i = 0; i < this->_length; i++) {
            std::cout << getStdIn(i) << " " << getStdOut(i) << std::endl;
        }
        std::cout << "]" << std::endl;
    }
};

void trace_cmd(struct cmdline *l) {

    int i, j;

    /* Display each command of the pipe */
    printf("trace_cmd\n");
    for (i = 0; l->seq[i] != nullptr; i++) {
        char **cmd = l->seq[i];
        printf("seq[%d]: ", i);
        for (j = 0; cmd[j] != nullptr; j++) {
            printf("(%s) ", cmd[j]);
        }
        printf("\n");
    }
}

void change_dir(char *param) {
    char *curr_dir;
    char buf[1024];
    char *path;
    if ((curr_dir = getcwd(nullptr, 0)) == nullptr) {
        perror("cd failed (getcwd)");
        return;
    }
    if (!param) path = getenv("HOME");
    else if (strncmp(param, "/", 1)) {
        if ((strlen(curr_dir) + strlen(param) + 2) > 1024) {
            perror("cd failed (path too large)");
            free(curr_dir);
            return;
        }
        path = strcpy(buf, curr_dir);
        path = strcat(path, "/");
        path = strcat(path, param);
    } else path = param;
    //printf("%s\n",path);
    if (chdir(path)) {
        perror("cd failed (chdir)");
        chdir(curr_dir);
    }
    free(curr_dir);
}


int main() {
    int spid, status;
    struct cmdline *l;
    char ***seq;

    while (true) {
        std::cout << "$ ";

        l = readcmd();
//        trace_cmd(l);
        seq = l->seq;

        if (!*seq) continue;
        if (!strcasecmp(**seq, "exit")) {
            return 0;
        }
        if (!strcasecmp(**seq, "cd")) {
            change_dir((*seq)[1]);
            continue;
        }

        auto linesList = new DynamicList<char **>(seq);
        if (linesList->length() == 1) {
            int pid = fork();
            if (pid == 0) {
                execvp(linesList->get(0)[0], linesList->get(0));
            } else {
                waitpid(pid, nullptr, 0);
            }
        } else {
            auto pipedesList = new PipeList(linesList->length() - 1);
            int pidMain = fork();
            if (pidMain == 0) {
                int pid = fork();
                for (int i = 0; i < linesList->length(); i++) {
                    if (pid == 0) {
                        pid = fork();
                    } else {
                        if (i == 0) {
                            pipedesList->dup2StdOut(0);
                            pipedesList->closeAll();
                            execvp(linesList->get(i)[0], linesList->get(i));
                        } else if (i == linesList->length() - 1) {
                            pipedesList->dup2StdIn(i - 1);
                            pipedesList->closeAll();
                            execvp(linesList->get(i)[0], linesList->get(i));
                        } else {
                            pipedesList->dup2StdOut(i);
                            pipedesList->dup2StdIn(i - 1);
                            pipedesList->closeAll();
                            execvp(linesList->get(i)[0], linesList->get(i));
                        }
                        waitpid(pid, nullptr, 0);
                        exit(0);
                    }
                }
                pipedesList->closeAll();
                exit(0);
            } else {
                pipedesList->closeAll();
                waitpid(pidMain, nullptr, 0);
            }
        }
    }
}


