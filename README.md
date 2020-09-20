# IntelFPGA_AzureOTA

このリポジトリでは，[実機貸出有！【オンライン】FPGA回路をリモートで書換→壮大なLチカ！インテルFPGAハンズオン#1](https://algyan.connpass.com/event/185328/)のフォローアップを受け付けています．

ハンズオンを自習してみて分からないことや詰まるところがありましたら，本リポジトリのIssueでお知らせください．    
https://github.com/algyan/IntelFPGA_AzureOTA/issues

また，ハンズオンに使用したソースコードや設計データも公開しています．

## ハンズオンの内容とテキスト

### [事前準備](https://drive.google.com/drive/folders/1Mz-QgMngeigpSAAAbzH-Ij0Q1DjbFHrC)

- [A. 用意するものと環境構築の手順](https://docs.google.com/document/d/13IKhIke5OjhjnLtdbKG3zMS6vxFePiYeyQTrPQs6wIQ)
- [B. Visual Studio Codeと拡張機能のインストール](https://docs.google.com/document/d/1vq3XFSypVGe6OZmN2M_jVGLmFW-dRD6NSIxcHGb6x4E)
- [C. Quartus Prime 18.1 Lite Editionのインストール](https://docs.google.com/document/d/10LPKzH9tKJ46EbwCKsPj41ujHwLx2zjyCdrZkuqNjAA)
- [D. Docker Desktopのインストール](https://docs.google.com/document/d/1zkJdRksS40nzDYClpdbrPxL1NBTac_Yvl0ji4BcIZZE)
- [E. カーネルイメージの書き込み(作成)](https://docs.google.com/document/d/1GEjcUaTV_shHZK85E4NPApeCh5uE9fn6IFqwMFVmadQ)
- [F. ボードの動作と接続の確認](https://docs.google.com/document/d/1gbOAJ3PUQfB5RaPPFQnbc8KhEpiXzRj-bIS1woVVOLE)


### [ハンズオン](https://drive.google.com/drive/folders/11JXvNV2e-gBMkFg2nv7VRUpmu0bFY4PF)

- [A. 開発の流れとハンズオン内容の紹介](https://docs.google.com/document/d/1Ar6TJoJiSOAGvc-f857OQ-FPKgzNGUQP90U-sSkAPpw)
- [B. DE10-Nanoボードでできること](https://docs.google.com/document/d/1O_U2eXpAu7FEMmwh6ZSPJTPtvuaxXJbNWUEQQfQLb3M)
- [C. Microsoft Azureの設定とIoT Edgeデバイスの登録](https://docs.google.com/document/d/17Cv8G8u0Gno1lVB1vXOASe3yWr6k62_uR_0-vy8kd1o)
- [D. Visual Studio Codeの設定](https://docs.google.com/document/d/19e6CicSQ1Ed2A_cDXXEczhKHX3UIpj9g6UhCE5ON6YM)
- [E. IoT Edgeモジュールの開発](https://docs.google.com/document/d/1fJ6fDUywMOkrS3sSzGaZzNwxVXnHjZbygylWARI1mVo)
- [F. FPGA回路から設計するには？](https://docs.google.com/document/d/1KIODum_KMib_BYQagTgrHQbNho3FVIhb-RZ6AjvQ70A)
- [G. Azureサービスの利用料金とハンズオンの後始末](https://docs.google.com/document/d/1O96qw5h9BWHM8dWzEM3Uw5sXmbZesg-VSRyy_rfol7M)

## ソースコード・設計データ

- AlgyanOverlay/
  - B.5.で実施するボード単体でのDevice Tree Overlay用のファイル（カーネルイメージに同梱済み）
- quartus_project/
  - LチカなFPGA回路のQuartus設計プロジェクト
- vscode_project/
  - G-Sensor/ ： C.7.でDocker HubからデプロイするDE10NanoGSensorモジュールのプロジェクト
  - L-Chika/ ： E.3.で実装するAwesomeLchikaなIoT Edgeモジュールのプロジェクト
    - E.8. のプロジェクトはmain branchにはマージしていません．[vsc_l-chika_dtc branch](https://github.com/algyan/IntelFPGA_AzureOTA/tree/vsc_l-chika_dtc/vscode_project)をご参照ください

