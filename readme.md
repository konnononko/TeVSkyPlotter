# TeV Sky Plotter

ROOT (Cern) macro to create a TeV gamma-ray source catalog plot.

Maybe it is useful for the PhD thesis figure etc.

![](https://github.com/konnononko/TeVSkyPlotter/blob/master/example/mytevsky.png)


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
Maybe it is good to sort by Type.

```
$> cat copyfromtevcat.txt

IC 310  03 16 43.0      +41 19 29       AGN (unknown type)
PKS 0625-35     06 26 58.2      -35 29 50       AGN (unknown type)
OJ 287  08 54 49.1      +20 05 58.89    BL Lac (class unclear)
HESS J0632+057  06 33 00.8      +05 47 39       Binary
HESS J1832-093  18 32 50        -09 22 36       Binary
HESS J1018-589 A        10 18 58        -58 56 43       Binary
...
```

Then change the list format to be separeted with space-comma-space.

```
$> cat copyfromtevcat.txt | sed -e 's/\t/\ \,\ /g' -e 's/$/\ \,/' > copyfromtevcatedit.txt
$> cat copyfromtevcatedit.txt

IC 310 , 03 16 43.0 , +41 19 29 , AGN (unknown type) ,
PKS 0625-35 , 06 26 58.2 , -35 29 50 , AGN (unknown type) ,
OJ 287 , 08 54 49.1 , +20 05 58.89 , BL Lac (class unclear) ,
HESS J0632+057 , 06 33 00.8 , +05 47 39 , Binary ,
HESS J1832-093 , 18 32 50 , -09 22 36 , Binary ,
HESS J1018-589 A , 10 18 58 , -58 56 43 , Binary ,
...
```


## Acknowlegement

This repository has made use the TeVCat online source catalog (http://tevcat.uchicago.edu).

