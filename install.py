# -*- coding:UTF-8 -*-
#! /usr/bin/python3
import shutil, os

if __name__ == "__main__":
    file1 = "MyTranslate"
    dir1 = "style"
    dir2 = "language"

    srcDir = "."
    dstDir = input("Enter install directory path:")
    if (os.path.exists(dstDir)):
        print("Destination directory has existed.")
    else:
        os.mkdir(dstDir)
        try:
            shutil.copy2(srcDir + "/" + file1, dstDir + "/" + file1)
            shutil.copytree(srcDir + "/" + dir1, dstDir + "/" + dir1)
            shutil.copytree(srcDir + "/" + dir2, dstDir + "/" + dir2)
        except Error as e:
            print(str(e))
        else:
            print("finish copying")
