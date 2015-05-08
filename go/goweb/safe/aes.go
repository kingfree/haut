package main

import (
	"crypto/aes"
	"crypto/cipher"
	. "fmt"
	"os"
)

var commonIV = []byte{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}

func main() {
	// 原文本
	pliantext := []byte("遊ぼうよ、和と")
	if len(os.Args) > 1 {
		pliantext = []byte(os.Args[1])
	}
	Println("原文本:", string(pliantext))
	// 加密盐 长度必须为 16B(128b), 24B(192b), 32B(256b)
	key_text := "最高の奇跡に乗り込め!!"
	if len(os.Args) > 2 {
		key_text = os.Args[2]
	}
	Println("盐", len(key_text), ":", key_text)
	// 加密算法
	c, err := aes.NewCipher([]byte(key_text))
	if err != nil {
		Printf("错误：NewCipher(%dB) = %s\n", len(key_text), err)
		os.Exit(1)
	}
	// 实行加密
	cfb := cipher.NewCFBEncrypter(c, commonIV)
	ciphertext := make([]byte, len(pliantext))
	cfb.XORKeyStream(ciphertext, pliantext)
	Printf("%s=>%x\n", pliantext, ciphertext)
	// 实行解密
	cfbdec := cipher.NewCFBDecrypter(c, commonIV)
	gettext := make([]byte, len(pliantext))
	cfbdec.XORKeyStream(gettext, ciphertext)
	Printf("%x=>%s\n", ciphertext, gettext)
}
