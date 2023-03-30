

一个基于muduo+protobuf实现的分布式RPC框架

1、安装protobuf：apt-get install libprotobuf-dev protobuf-compiler  # 安装protobuf编译器是成功的
2、手动编译安装muduo库
3、安装Java和zookeeper：apt-get install openjdk-8-jre 
                        apt-get install zookeeper zookeeperd
                        apt-get install libzookeeper-mt-dev # 安装zookeeper多线程C API
4、 查找zookeeper安装路径从而启动zk服务器和客户端: find / -name *zookeeper*