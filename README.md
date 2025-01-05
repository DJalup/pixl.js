# Pixl.js

This is a clone based on the original [Pixl.js](http://www.espruino.com/Pixl.js), the main function is to simulate Amiibo.

Changes made to this fork, All changes were made to pixl.ble.js

1. Reconnection: Automatically reconnect on disconnection.
2. MTU: Negotiate maximum MTU size.
3. Retry Logic: Retry data transmission if it fails.
4. Throttling: Add delays between chunks to stabilize file transfers.
5. Error Handling: Emit detailed error messages.
6. RSSI Monitoring: Monitor signal strength to detect weak connections.
7. Logs: Add detailed logs for debugging.

## Images

![image](https://github.com/solosky/pixl.js/blob/main/assets/pixljs-3.jpg)

![image](https://github.com/solosky/pixl.js/blob/main/assets/pixljs-4.jpg)

![image](https://github.com/solosky/pixl.js/blob/main/assets/pixljs-5.jpg)

## Documentation

* [Chinese Documentation](docs/zh/README.md)

* [English Documentation](docs/en/README.md)

* [Italian Documentation](docs/it/README.md)

## Credits

* [FlipperZero Firmware](https://github.com/flipperdevices/flipperzero-firmware)

* [mlib](https://github.com/P-p-H-d/mlib)
* [TLSF](https://github.com/mattconte/tlsf)
* [cwalk](https://github.com/likle/cwalk)
* [SPIFFS](https://github.com/pellepl/spiffs)
* [ChameleonUltra](https://github.com/RfidResearchGroup/ChameleonUltra)

## Contribution

* Special thanks to @Caleeeeeeeeeeeee for perfecting the bootloader.

* Special thanks to @白橙 for making the shell.

* Special thanks to @impeeza for providing document translation.

## Discussion Group

Domestic users can join QQ group 109761876 for communication.

## Office Channel

Where do you find the community?

* [Pixl.js community discord server](https://discord.gg/4mqeQwcAB2)

## Statement

This project is an open source project, for study and research purposes only, please do not use it for commercial purposes. <br />
Amiibo is a registered trademark of Nintendo, and NTAG21X is a registered trademark of NXP.

The sources of the built-in Amiibo database are as follows:

* [amiiloop](https://download.amiloop.app/)

* [AmiiboAPI](https://www.amiiboapi.com/)

The source code does not contain any Nintendo copyrighted resources (such as related keys, Amiibo original data, etc.).

# License

This project is released based on the GPL 2.0 License, please follow the License agreement when using it.

* If you make changes to the project, you need to publish the modified source code
* The published source code must be published using the same license

## Tips

Amiibo unlimited flash requires searching for the key_retail.bin file online and uploading it to the root directory of the disk before it can be used.
