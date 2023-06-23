

# копирует заданные файлы в каталог назначения
defineTest(copyToDestDir) {
    files = $$1
    dir = $$2
    # заменить слеши в пути назначения для Windows
    win32:dir ~= s,/,\\,g

    for(file, files) {
        # заменить слеши в исходном пути для Windows
        win32:file ~= s,/,\\,g

        QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($$file) $$shell_quote($$dir) $$escape_expand(\\n\\t)
    }
files = $$files

    message($$QMAKE_POST_LINK)

    export(QMAKE_POST_LINK)
}

#copyToDestDir($$PWD/settings.ini, $$OUT_PWD/$${VARIANT}/)
#copyToDestDir($$PWD/xdf, $$OUT_PWD/$${VARIANT}/xdf/)
copyToDestDir($$PWD/xml, $$OUT_PWD/$${VARIANT}/xml/)


