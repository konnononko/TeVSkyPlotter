# TeV Sky Plotter

ROOT (Cern) macro to create a TeV gamma-ray source catalog plot.

Maybe it is useful for the PhD thesis figure etc.

![](https://github.com/konnononko/TeVSkyPlotter/example/mytevsky.png)


## Features

- ROOT macro example to create all-sky plot with galactic coordinate.
- Create all-sky plot from TeVCat-like source list.
- Fancy your own all-sky plot with very-high-energy gamma-ray sources.


## Prerequisite

Install ROOT (https://root.cern.ch/).


## Usage

```
$> root myTevSky.C
```

### Update Source List

We can customize the source list.

Here we copy and paste from TeVCat (http://tevcat.uchicago.edu/) table.
You can prepare your own list if you want.

Copy and paste to copyfromtevcat.txt from TeVCat table selecting "Name", "Ra", "Dec", and "Type".

```
$> cat copyfromtevcat.txt

CTA 1   00 06 26        +72 59 01.0     PWN
SHBL J001355.9-185406   00 13 52.0      -18 53 29       HBL
Tycho   00 25 27        +64 10 50       Shell
KUV 00311-1938  00 33 34.2      -19 21 33       HBL
1ES 0033+595    00 35 16.8      +59 47 24.0     HBL
NGC 253 00 47 34.3      -25 17 22.6     Starburst
...
```

Then change the list format to be separeted with space-comma-space.

```
$> cat copyfromtevcat.txt | sed -e 's/\t/\ \,\ /g' -e 's/$/\ \,/' > copyfromtevcatedit.txt
$> cat copyfromtevcatedit.txt

CTA 1 , 00 06 26 , +72 59 01.0 , PWN ,
SHBL J001355.9-185406 , 00 13 52.0 , -18 53 29 , HBL ,
Tycho , 00 25 27 , +64 10 50 , Shell ,
KUV 00311-1938 , 00 33 34.2 , -19 21 33 , HBL ,
1ES 0033+595 , 00 35 16.8 , +59 47 24.0 , HBL ,
NGC 253 , 00 47 34.3 , -25 17 22.6 , Starburst ,
```


## Acknowlegement

This repository has made use the TeVCat online source catalog (http://tevcat.uchicago.edu).

