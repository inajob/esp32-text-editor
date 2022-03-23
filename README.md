# ESP32 hid keyboard editor

M5StackにUSB Host Shieldを接続し、キーボードをつなげた状態で動作するエディタです。

エディタのコア部分をlibに分離してあるので、例えばCardKBなどに対応させるのは簡単だと思います。


絶賛作業中です。

ローマ字入力により日本語が入力できたりします。

## Memo

### USB Host Shieldとの接続

- https://okiraku-camera.tokyo/blog/?p=8333
  - USB Host Shieldに改造が必要です。
- 改造が面倒な場合は、[M5Stack用MAX3421E搭載 USBモジュール](https://amzn.to/3D4Y7tX)を使うと良いです。


### Setup dictionary

http://openlab.ring.gr.jp/skk/dic-ja.html から辞書をダウンロードしてください。

辞書が大きいと、かな漢字変換が非常に遅くなるので「SKK-JISYO.S」がおススメです。

ファイルの中身をUTF-8、改行コードLFに変換したうえで、`data/SKK-JISYO.S.txt`に保存してください。

下記コマンドラインで辞書をSPIFFSに格納してください。

```
$ platformio run -e esp32 --target uploadfs
```

### Build and upload

```
$ platformio run -e esp32 --target upload
```

### Test

```
$ platformio test -e native
```
