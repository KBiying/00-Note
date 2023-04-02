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