# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test_nm.py                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: arobion <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/01 15:45:46 by arobion           #+#    #+#              #
#    Updated: 2019/07/12 10:49:35 by arobion          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

import subprocess
import argparse
from color_defines import *

files_fail = []
files_segv = []

def launch_test(dir_name, exec_cmd):
    nb_tests = 0
    nb_success = 0
    nb_fail = 0
    nb_segv = 0
    list_files = subprocess.run(["ls", dir_name], capture_output=True).stdout.decode("utf-8").split("\n")
    for files in list_files:
        if files != "":
            nb_tests += 1
            mein_out = subprocess.run([exec_cmd + " " + dir_name + files], shell=True, stdout=subprocess.PIPE)
            if mein_out.returncode != 0 and mein_out.returncode != 1:
                print("{} => {}SEGV{} ".format(files, FAIL, ENDC))
                nb_segv += 1
                files_segv.append(files)
            else:
                true_out = subprocess.run(["nm" + " " + dir_name + files], shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                if true_out.stdout == mein_out.stdout or true_out.stderr != b'':
                    print("{} => {}SUCCESS{}".format(files, OKGREEN, ENDC))
                    nb_success += 1
                else:
                    print("{} => {}FAIL{}".format(files, WARNING, ENDC))
                    nb_fail += 1
                    files_fail.append(files)
    return [nb_tests, nb_success, nb_fail, nb_segv]


def test_diff(exec_cmd, dir_name):
    nb_test = 0
    list_files = subprocess.run(["ls", dir_name], capture_output=True).stdout.decode("utf-8").split("\n")
    for files in list_files:
        if files != "":
            true_out = subprocess.run(["nm" + " " + dir_name + files], shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            with open("true_out.log", "w") as f:
                f.write(true_out.stdout.decode())
            while True:
                mein_out = subprocess.run([exec_cmd + " " + dir_name + files], shell=True, stdout=subprocess.PIPE)
                if mein_out.stdout != true_out.stdout:
                    with open("mein_out.log", "w") as f:
                        f.write(mein_out.stdout.decode())
                    print("DIFF detected in {} tests".format(nb_test))
                    return
                nb_test += 1
                if nb_test % 100 == 0:
                    print("{} tests".format(nb_test))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-d", "--directory", type=str, help="optional, choose a directory")
    parser.add_argument("-a", "--all", action="store_true", help="optional, launch on all dir")
    parser.add_argument("-t", "--test", action="store_true", help="optional, launch infinitely on one dir until diff")
    args = parser.parse_args()
    exec_cmd = "./ft_nm"

    print("compilation and copying of ft_nm")
    subprocess.run(["rm -f ft_nm"], shell=True)
    ret = subprocess.run(["cd .. && make re && cp ft_nm tests/."], shell=True, stdout=subprocess.PIPE)
    if ret.returncode != 0:
        print("\t{}compilation FAILURE {}".format(FAIL, ENDC))
        return
    else:
        print("\t{}compilation SUCCESS {}\n".format(OKGREEN, ENDC))


    if args.all == False:
        if args.directory:
            dir_name = args.directory
        else:
            dir_name = "binary_dir/"
        if args.test == True:
            test_diff(exec_cmd, dir_name)
            return 
        nb_tests, nb_success, nb_fail, nb_segv = launch_test(dir_name, exec_cmd)
    else:
        dir_name = "/usr/bin/"
        nb_tests, nb_success, nb_fail, nb_segv = launch_test(dir_name, exec_cmd)
        dir_name = "/bin/"
        nb_tests_tmp, nb_success_tmp, nb_fail_tmp, nb_segv_tmp = launch_test(dir_name, exec_cmd)
        nb_tests, nb_success, nb_fail, nb_segv = nb_tests_tmp + nb_tests, nb_success_tmp + nb_success, nb_fail_tmp + nb_fail, nb_segv_tmp + nb_segv
        dir_name = "/usr/lib/"
        nb_tests_tmp, nb_success_tmp, nb_fail_tmp, nb_segv_tmp = launch_test(dir_name, exec_cmd)
        nb_tests, nb_success, nb_fail, nb_segv = nb_tests_tmp + nb_tests, nb_success_tmp + nb_success, nb_fail_tmp + nb_fail, nb_segv_tmp + nb_segv

    print("\nRESUME\n\tnb tests : {}\n\tnb success : {}{}{}\n\tnb_fail : {}{}{}\n\tnb_segv : {}{}{}".format(nb_tests, OKGREEN, nb_success, ENDC, WARNING, nb_fail, ENDC, FAIL, nb_segv, ENDC))
    with open("logs.log", "w") as f:
        for elem in files_segv:
            f.write("segv: {}\n".format(elem))
        f.write("\n\n")
        for elem in files_fail:
            f.write("fail: {}\n".format(elem))



if __name__ == "__main__":
    main()
