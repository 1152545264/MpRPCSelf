一个基于muduo+protobuf实现的分布式RPC框架

1、安装protobuf：apt-get install libprotobuf-dev protobuf-compiler  # 安装protobuf编译器是成功的

2、手动编译安装muduo库

3、安装Java和zookeeper：

                                            apt-get install openjdk-8-jre 

                        apt-get install zookeeper zookeeperd
    
                        apt-get install libzookeeper-mt-dev # 安装zookeeper多线程C API

4、 查找zookeeper安装路径从而启动zk服务器和客户端: find / -name *zookeeper*



5、建立bin和build目录，并将test.conf拷贝至bin目录下

6、执行./autobuild.sh，完成编译生成

7进入bin目录，执行provider -i test.conf 或者FriendConsumer -i test.conf分别开启分布式服务示例服务器和客户端。
