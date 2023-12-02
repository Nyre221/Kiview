# Kiview
Kiview gives the user the ability to quickly preview different file types without the need to launch the default application.
### Supported file types:
.txt,.sh,.pdf,.doc,.docx,.odt,.ods,.xlsx,.xls,.csv,.odp,.ppt,.pptx,.png,.jpg,.jpeg,.kra,.svgz,.svg,.mp4,.mp3,.webm,.zip,.gz,.xz,.rar

## Known Issues
Currently due to the way it integrates with dolphin, the last item copied to the clipboard is modified and then restored. This isn't a problem if it's just text, but unfortunately it can't restore the contents if the last item copied was a folder or file.

## To create and install the flatpak package:

1) Open konsole
2) `git clone https://github.com/Nyre221/Kiview.git`
3) `cd ./Kiview`
4) `flatpak install io.qt.qtwebengine.BaseApp/x86_64/5.15-23.08`
5) `flatpak install org.kde.Sdk/x86_64/5.15-23.08`
6) `flatpak-builder --user --install --force-clean  build-dir ./com.nyre.kiview.json`
7) RUN: `flatpak run org.nyre.kview`

## How it works
To use it you need to set a keyboard shortcut via plasma settings and invoke it while a dolphin window is active.   
Any key combination is fine to activate the shortcut and it is only necessary to enter this command in the appropriate section: `kiview` (for the non-flatpak version) or `flatpak run com.nyre.kiview` (for the flatpak version).

#### Example:
![Screenshot-30-11-2023-CET-11](https://github.com/Nyre221/Kiview/assets/104171042/2d5cc6c8-a217-40e9-b2a8-f6e28ed9c99a)



![Screenshot-30-11-2023-CET-9](https://github.com/Nyre221/Kiview/assets/104171042/4e06a068-3d51-4b57-adcf-f42693b92e18)

![Screenshot-30-11-2023-CET-10](https://github.com/Nyre221/Kiview/assets/104171042/37b1fb2c-5e89-48fb-b5cf-25750734fd14)


### If you want to support my work:
https://www.paypal.com/donate/?hosted_button_id=J7QU55MMUP4G4
