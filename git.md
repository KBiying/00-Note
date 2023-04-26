# 保命四步

git add

git commit

git pull合并冲突

git push



# Git使用出现git@github.com: Permission denied (publickey). 处理

第五部，ssh -T [git@github.com](mailto:git@github.com) 测试一下通不通，通了显示如下
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190521165223872.png)
即可正常使用。
不通就是
ssh-agent -s
ssh-add ~/.ssh/id_rsa 操作这两步。

# git@git.snrc.site: Permission denied (publickey,keyboard-interactive).



> The authenticity of host 'git.snrc.site (172.31.72.76)' can't be established. 
> ED25519 key fingerprint is SHA256:rcjTSxS9umuLv0hTsafVaKT2WiOE1ZmDioh2MTUpgso. 
> This key is not known by any other names 
> Are you sure you want to continue connecting (yes/no/[fingerprint])? yes 
> Warning: Permanently added 'git.snrc.site' (ED25519) to the list of known hosts. 
> git@git.snrc.site: Permission denied (publickey,keyboard-interactive). 
> fatal: Could not read from remote repository. 
>
> Please make sure you have the correct access rights 
> and the repository exists.

解决方法同上一个问题

ssh -T git@git.snrc.site

ssh-agent -s
ssh-add ~/.ssh/id_rsa_gops 操作这两步。