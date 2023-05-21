信用卡号的格式特征
信用卡号由13至19位数字构成，前6位为发卡行标识代码，用以识别发行该银行卡的机构，由IOS组织分配；中间的6至12位为发卡行自定位，由发卡机构自行分配，用以识别个人账户；最后一位为校验码，采用Luhn算法计算得出，具有防伪功能。
Luhn算法简单描述为：
	信用卡号若为偶数位，则奇数位的权重为2，偶数位的权重为1；信用卡号若为奇数位，则奇数位的权重为1，偶数位的权重为2。
	将卡号的每一位数字与权重相乘，结果若大于9，则减去9。
	将处理后的卡号的所有位求和，若总和能被10整除，则确定是一个合法的信用卡号。
例如：某信用卡号是一个16位数字：
5218 9912 1147 7929
从左至右取奇数位数字，得到：
5 1 9 1 1 4 7 2
每个数字乘以2，若结果大于9，则减去9，处理之后得到：
1 2 9 2 2 8 5 4

将这些数字分别相加，再加上原信用卡号的偶数位数字，得到：
1+2+9+2+2+8+5+4+2+8+9+2+1+7+9+9=80
80能被10整除，验证这是一个合法的信用卡号。
信用卡号进行保留格式加密后必须是一个合法的信用卡号，前6位是正确的IOS组织分配的BIN号码，最后校验码符合校验规则，保证是一个合法的信用卡号。
三、编号类敏感信息的FPE方案
根据身份证号和信用卡号的格式特征，提出具有分段约束的编号类敏感信息的FPE方案（NumFPE）。
由于某发卡行对自定义位可再进行分段约束，如交通银行信用卡号的7至9位为分行区间号，11至13位为顺序号等。不失一般性，设待加密编号明文x由n位字符构成，n-1位本体码分为k个字段，第n位为校验码，如下表所示：
分段序号	1	2	...	i	...	校验码
分段范围	1~n_1	n_1+1~n_1+n_2	...	n_1+n_2+...+n_(i-1)+1~n_1+n_2+...+...n_i	...	n
位数	n_1	n_2	...	n_i	...	1
其中各分段可分为两类：1.特征码，即几位数字在特定范围内的编码，如身份证号中的地址码、信用卡号中的发卡行标识等。实际上，身份证号中的8位日期也属于此范畴。2.顺序码，即某几位数字没有特定范围，属于几位十进制数的所有自然数范围，身份证号和信用卡号均有此分段。
最后1位校验码根据编号类型及其校验规则，由本体码计算生成。
根据格式特征对本体码的各分段进行不同的保留格式加密计算，然后将加密结果合并，最后根据校验规则由本体码的加密结果计算得到校验码。1.对于特征码字段，可采用RtE、FFX和CTE等编码后加密算法进行保留格式加密，将特征码的特定范围内映射为整数，对整数进行加密，加密结果再映射回合法的特征码。2.对于顺序码字段，可采用整数FPE（Prefix、FFXEM等）进行保留格式加密，将顺序码直接转换成整数类型，对整数加密后，再转换回字符类型。
分段加密过程如下图：
 ![image](https://github.com/hack-scan/Credit-card-encryption/assets/36232689/9e5a6709-d7c9-43d7-b5c6-76fc77bf0b42)

NumFPE具体的算法描述：
算法Setup：
	确定编号类型type，编号位数n，各特征码分段域Domain_i，校验码计算函数CalCheckCode（•）；
	确定FPE算法及其参数ParaofFPE；
	确定密钥key。
算法encryption：
输入明文x，密钥key，编号类型type，号码位数n，FPE算法参数ParaofFPE，各特征码分段域Domain_i，校验码计算函数CalCheckCode（•），输出密文y。
加密的具体过程：1.根据编号类型，将n-1位本体码划分为若干分段section_i。2.对各分段section_i依次加密：若为特征码，则利用CTE及密钥key，将特征码范围作为消息空间Domain_i，对n_i位特征码进行保留格式加密，加密结果为y_i；若为顺序码，则将该顺序码转换成整数类型，利用整数FPE及密钥key对其进行保留格式加密，加密结果为y_i。3.将各分段加密结果合并为n-1位字符串y^'，即为本体码密文。4.根据编号的校验规则，由本体码密文计算得到校验码密文CheckCode’。5.将本体码密文y^'与校验码密文CheckCode’连接即为最终编号密文y。加密算法如图：
算法decryption：
算法encryption过程的逆过程：
 ![image](https://github.com/hack-scan/Credit-card-encryption/assets/36232689/09079c3d-5512-4b60-81db-c237c378b59c)

四、安全性和效率分析
1.安全性分析：
编号类敏感信息FPE方案的基础加密模块采用现有的FPE算法，这些FPE算法均具有可证明安全性，在已有的研究中已经证明达到了PRP安全。
	效率分析：
算法NumFPE在k个分段内进行保留格式加密，需要进行k次FPE计算，若 保留格式加密时间记为Te，校验码计算时间记为Tc，则算法加密时间主要为kTe+Tc。而针对实际应用中的各类编号，k值较小，且各分段内的消息空间规模较小，因此该算法具有稳定性且较高的效率。
	总结
现有FPE算法只针对常用数据类型（数值型、日期型和字符型）给出解决方案，对于数据完整性约束问题并未考虑。尤其是对身份证号、银行卡号、信用卡号等存在分段约束的号码类数据尚未提出相应的FPE解决方案。事实上，无论是数据存储、网络传输等实际应用场景，还是数据测试等非生产环境，对敏感数据加密时，都不仅要考虑保留类型、长度等格式，以防止数据库存储结构和应用程序的改动，还应保留分段格式特征，否则将会因为不满足数据完整性约束而导致数据库存储异常或数据处理异常。
本节针对此问题，提出了存在分段约束的编号类的FPE方案，以满足应用需求。首先以身份证号和信用卡号为例分析了编号类敏感数据的格式特征，发现编号类敏感数据存在分段约束的格式特点。然后提出编号类数据的FPE统一方案，并分析了该方案的安全性和效率。