from aip import AipSpeech

""" 你的 APPID AK SK """
APP_ID = '32220461'
api_key = 'Es9Wt1MMQTp8PBGNdaZS6ZB8'
secret_key = 'vAv50cCDGEYHlDbbLhU7uOzPYpY0QXLy'
client = AipSpeech(APP_ID, api_key, secret_key)


# 读取文件
def get_file_content(filePath):
    with open(filePath, 'rb') as fp:
        return fp.read()

# 识别本地文件
res = client.asr(get_file_content('test1.wav'), 'wav', 16000, {
    'dev_pid': 1537,
})

print(res["result"])
