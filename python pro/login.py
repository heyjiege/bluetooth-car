# -*- coding: utf-8 -*-

"""
基础需求：
让用户输入用户名密码
认证成功后显示欢迎信息
输错三次后退出程序

升级需求：
可以支持多个用户登录 (提示，通过列表存多个账户信息)
用户3次认证失败后，退出程序，再次启动程序尝试登录时，还是锁定状态（提示:需把用户锁定的状态存到文件里）

user_info = {
    "pizza": ["pizza123", "1"],
    "alex": ["alex123", "0"]
}
"""

user_info = dict()
exit_flag = False
attemp_times = 0


user_file = open("user_info", "r+")

for line in user_file:
    line = line.strip().split(',')
    user_info[line[0]] = line[1:]

while not exit_flag:
    username = input("Please input your account: ")
    if username in user_info.keys():
        if user_info[username][1] == "1":
            print("Your account has been locked!")
            break
        while attemp_times < 3:
            password = input("Please input your password: ")
            if password == user_info[username][0]:
                print("Welcome back %s!" % username)
                exit_flag = True
                break
            else:
                attemp_times += 1
                print("Wrong password!")
        else:
            user_info[username][1] = "1"
            user_info_to_write_list = list()
            user_file.seek(0)
            user_file.truncate()
            for account in user_info:
                new_user_info = [account, user_info[account][0], user_info[account][1]]
                user_info_str = ','.join(new_user_info)
                user_info_to_write_list.append(user_info_str)
            user_info_to_write_str = '\n'.join(user_info_to_write_list)
            user_file.write(user_info_to_write_str)
    else:
        attemp_times += 1
        print("Wrong account!")
    if attemp_times == 3:
        print("You have only 3 times! Bye.")
        exit_flag = True
