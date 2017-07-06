参赛程序是CTF.exe，支持Windows 7 32/64位到Windows 10 Build 15063.250。XP未测试，应该也能运行
为了保持简单与可移植性，此程序无图形界面，不需要任何额外的运行库
如果key输入正确，会打印字符串“Yeah! You've got it”并自动退出
杀毒软件可能会报毒，现在的杀软看到壳就报毒。。。这个问题在下也很头疼。有时候刚运行就被Windows Defender删掉了。。。具体报毒情况参见：
https://www.virustotal.com/en/file/b5b1fb5642d811c0a0059ba41398ddac2716057ff01b6531dd814fb927d16369/analysis/1493728563/
http://r.virscan.org/report/e02d220087c3b8bc2cdc4e16f7f72bbc

正确的密钥是ToBeOrNot2Be

CrackMe目录下是主程序的代码，以及VS2015的工程文件
PEZEncrypt目录下是在下为本次比赛随手写的一个简单的壳，更通用的代码已经被在下放在https://github.com/emc2314/PEZEncrypt
References目录下是参考文献。即这个程序用到的一些技术来源
Brainfuck目录下是与Brainfuck虚拟机相关的文件
Bin目录下是一些手工处理的结果。libbf.exe是libbf的编译结果。CrackMe.exe是CrackMe工程编译所得，CrackMe-stolen是手工将程序入口点的几条指令去除后的结果。Project1.exe是PEZEncrypt的编译结果。将CrackMe-stolen.exe拖到Project1.exe上即为最终结果
CTF.exe.checksum是CTF.exe的哈希，CTF.exe.checksum.asc是用在下的GPG私钥对CTF.exe.checksum的签名。均为ASCII文件

程序大概分四个层次：
1. 加壳。加壳很简单，用XTEA算法对.text和.data区段进行加密，然后运行的时候自动解密。对输入表无处理。壳的代码中使用了SEH来混淆程序流程，并且检测CONTEXT中的硬件断点。为了防止ESP定律，壳的开头首先是一个虚假的pushad，然后把popad藏在花指令里，再sub esp, a_certain_value，然后才是真的pushad，在最后的跳转前add esp, a_certain_value。且将SEH藏在这个pushad的结构里，使得硬件断点（如果有的话）提前多次触发。为防止内存断点，在运行过程中将栈所在的页设为可读可写，覆盖可能的NO_ACCESS属性。在SEH处理中设置SetUnhandledExceptionFilter，以此检测调试器。并且运行中有两次内存校验，防止0xCC。
2.CrackMe。这个程序中的字符串都被异或了一个随机值（编译时），并在每次调用重要函数的时候使用模板元编程构成一个有限状态机，在状态转化的时候检测调试器，如果没有才会完成调用
3.Brainfuck。真正的校验流程被隐藏在著名的Brainfuck虚拟机中。这个语言只有[ ] + - < > . , 8条指令，CrackMe中实现了一个虚拟机，BF的代码被放在.rsrc段中（为了降低难度，并未加密字符串），注册流程大概是如果输入字符串长度为12，就将12个字符的ASCII码（96个bit）输入VM中运行（见CrackMe\lfsr.cpp）。然后VM会输出一个结果，CrackMe会将这个结果与预设的一个数组比较，如果相同就认为正确。
4.LFSR。注册机制是一个线性反馈移位寄存器（LFSR），这个是原来生成流密码的基本模型。这个模型存在很多问题所以现在渐渐被淘汰。本程序利用了其中一个漏洞：代数攻击（见References\lfsr algebraic attack.pdf）。具体的漏洞利用代码尚未放在文件夹中，如果此程序被本次比赛选中，在下再行公开。

破解思路：
说实话自己写的程序自己都觉得难破解。。。看着自己的源代码大概有这几个步骤：
1.脱壳，找到OEP直接dump，怎么找OEP？慢慢跟踪呗。几个反调试需要插件才能过，毕竟很小的壳，耐心一点就好了
2.识别主函数中的函数调用，找到几个重要函数（比如BF的VM相关代码）。这个主程序很小，也不难。
3.最难的就是弄清楚BF要做什么。这个先将BF还原为C代码（非常简单），然后通过窥孔优化等技术简化这个流程（其实最简单的方法就是通过各种优化能力强的编译器比如gcc, intel c++ compiler编译这个C代码，再反汇编回来）。然后在程序中导出那个与VM输出想对比的数组
4.最后就是破解lfsr了，这个通过论文中的方法，解一个3570（即12*7+（12*7）*（12*7-1）/2）元的线性方程组就行了。高斯消元法（或者用SAT的算法复杂度更低）的复杂度是N^3，大概几十秒就跑出来了。

最后，穷举的空间不大，只有10^26水平。但是这个程序我故意写的比较慢，所以想直接穷举的话不太可能