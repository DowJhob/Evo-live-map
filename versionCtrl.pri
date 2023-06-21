

BASE_GIT_COMMAND = git --git-dir $$PWD/.git --work-tree $$PWD
GIT_VERSION = $$system($$BASE_GIT_COMMAND describe --always --tags)
message("GIT_===============================================================!" $$BASE_GIT_COMMAND)
message("GIT_===============================================================!" $$GIT_VERSION)
DEFINES += GIT_VERSION=\\\"$$GIT_VERSION\\\"
VERSION = $$GIT_VERSION

win32 {
    VERSION ~= s/-\d+-g[a-f0-9]{6,}//
}
message($$VERSION)
