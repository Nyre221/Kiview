# Kiview
Kiview gives the user the ability to quickly preview different file types without the need to launch the default application.
### Supported file types:
.txt,.sh,.pdf,.doc,.docx,.odt,.ods,.xlsx,.xls,.csv,.odp,.ppt,.pptx,.png,.jpg,.jpeg,.kra,.svgz,.svg,.mp4,.mp3,.webm,.zip,.gz,.xz,.rar

## Installation
### Flathub
https://flathub.org/apps/io.github.nyre221.kiview  
From the terminal: `flatpak install io.github.nyre221.kiview`
### Kde Neon (manual update)
`wget -O kiview.deb  $(curl -s https://api.github.com/repos/Nyre221/Kiview/releases/latest | grep browser_download_url | cut -d '"' -f 4)`  
`sudo dpkg -i ./kiview.deb`  
`rm ./kiview.deb`
### OpenSuse Tumbleweed
`sudo zypper ar -f https://download.opensuse.org/repositories/home:/nyre21/openSUSE_Tumbleweed/home:nyre21.repo`  
`sudo zypper install kiview`

## How it works
To use it you need to set a keyboard shortcut via plasma settings and invoke it while a dolphin window is active.   
Any key combination is fine to activate the shortcut and it is only necessary to enter this command in the appropriate section: `kiview --shortcut` or `flatpak run io.github.nyre221.kiview --shortcut` (for the flatpak version).

#### Example:
![Schermata_20240403_172653](https://github.com/Nyre221/Kiview/assets/104171042/14ed7534-f2cc-4c92-85da-6ce7b1ccfd73)

![Screenshot-30-11-2023-CET-9](https://github.com/Nyre221/Kiview/assets/104171042/4e06a068-3d51-4b57-adcf-f42693b92e18)

![Screenshot-30-11-2023-CET-10](https://github.com/Nyre221/Kiview/assets/104171042/37b1fb2c-5e89-48fb-b5cf-25750734fd14)


### If you want to support my work:
https://www.paypal.com/donate/?hosted_button_id=J7QU55MMUP4G4
