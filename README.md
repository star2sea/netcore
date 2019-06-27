# 编译步骤

- 进入thirdparty目录，执行python thirdparty.py，下载并编译第三方依赖库
- 在netcore目录下创建build目录，并进入build目录下
- Windows系统执行 cmake .. -G "Visual Studio 15 2017 Win64"，在build目录下会生成netcore.sln,用vs打开，查看编译选项是否为RelWithDebInfo和x64，然后build工程即可
- Macos和Linux系统直接在build目录下执行cmake .. && make

# 示例

网络库使用方法可以参考test目录下的server_test.cpp 和 client_test.cpp，实现了最简单的客户端服务端通信。

