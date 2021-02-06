# susjoiner
susファイルを合成するツール。  
既存のsusエディタでハイスピ設定した譜面を出力しこのツールで合成すると、簡単にノーツ追い越し譜面が作成できます。

## コマンド
```
susjoiner <base SUS file>
```
- base SUS file
  - 合成のベースとなるsusファイル

## 使い方 (Windows)
1. ベースとなるsusファイルと合成したい(重ねたい)susファイルを同じフォルダに用意します。
```
注意: 合成したいsusファイルの名前は「tl.<任意の文字列>.sus」にしておく必要があります。
```
2. `susjoiner.exe` のアイコンにベースとなるsusファイルをD&Dします。(ターミナルで実行も可)
3. 合成済みの完成品が `<ベース譜面のファイル名>.joined.sus` の名前で出力されます。

![](https://user-images.githubusercontent.com/32731693/107115697-ca738300-68b1-11eb-9890-19cb287d8bc7.gif)

## ビルド
```
g++ -std=c++17 main.cpp -o susjoiner
```

## LICENSE
[GNU GENERAL PUBLIC LICENSE v3](LICENSE)