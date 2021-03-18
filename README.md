# ESP32 hid keyboard editor

M5StackにUSB Host Shieldを接続し、キーボードをつなげた状態で動作するエディタです。

エディタのコア部分をlibに分離してあるので、例えばCardKBなどに対応させるのは簡単だと思います。


絶賛作業中です。

ローマ字入力により日本語が入力できたりします。

## Memo

### USB Host Shieldとの接続

- https://okiraku-camera.tokyo/blog/?p=8333
  - USB Host Shieldに改造が必要です。

### Build and upload

```
$ platformio run -e esp32 --target upload
```

### Test

```
$ platformio test -e native
```
