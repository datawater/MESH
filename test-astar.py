#!/usr/bin/env python3

from os import system

def main():
    system("./mesh 2> mesh.dot")
    system("dot -Tpng mesh.dot -o mesh.png")
    system("feh mesh.png &")

    continue_ = input("continue? [y/n]")

    system("pkill -9 feh")
    system("rm mesh.dot mesh.png")

    if continue_ == "y":
        main()

if __name__ == "__main__":
    main()
