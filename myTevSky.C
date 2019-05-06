/*
 *	myTevSky.C
 *	KONNO Yusuke
 *	20161126
 *
 *	Plotter to Create Your Own Fancy:) TeV Sky from TeVCat Table
 */

#include <iostream>
#include <fstream>
#include <string>

#include <TCanvas.h>
#include <TGraph.h>
#include <THistPainter.h>
#include <TH2F.h>
#include <TMath.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TLatex.h>


#define MARK_TARGET_SOURCE


void Gal2Aitoffxy(double lon, double lat, double &x, double &y)
{
	THistPainter::ProjectAitoff2xy(lon,lat,x,y);
	x=-x;
}

double hms2deg(double hh, double mm, double ss)
{
	double deg = 360.*(hh/24.+mm/24./60.+ss/24./60./60. );
	return deg;
}
double dms2deg(double dd, double mm, double ss)
{
	double deg = dd + mm/60. + ss/60./60.;
	return deg;
}

void cel2gal(double ra, double dec, double &lon, double &lat) //all in degree
{
	double c = 1./180.*TMath::Pi();
	//galactic north pole
	double raG=192.85;
	double decG=27.128333333;
	//galactic center
	double raC=266.4;
	double decC=-28.929656275;

	lat=asin( sin(dec*c)*sin(decG*c) + cos(dec*c)*cos(decG*c)*cos((ra-raG)*c) );
	lat/=c;

	double J, K, Q;

	J=( sin(dec*c)*cos(decG*c) - cos(dec*c)*sin(decG*c)*cos((ra-raG)*c) )/cos(lat*c);
	K=asin( cos(dec*c)*sin((ra-raG)*c)/cos(lat*c) );
	Q=acos( sin(decC*c)/cos(decG*c) );
	K/=c;
	Q/=c;

	if(J<0) lon=Q+K-180;
	else lon=Q-K;

	if(lon<-180) lon+=360;
	if(lon>180) lon-=360;
}

Int_t myTevSky()
{
//Number of Grids
	const int nlon=13;
	const int nlat=7;
	const int ndiv=100;

	TGraph *glon[nlon];
	TGraph *glat[nlat];

	for(int i=0;i<nlon;i++)
	{
		glon[i]=new TGraph();
		for(int j=0;j<ndiv;j++)
		{
			double lonx=0,laty=0,lon=-180+360.*i/(nlon-1),lat=-90+180.*j/(ndiv-1);
			//THistPainter::ProjectAitoff2xy(lon,lat,lonx,laty);
			Gal2Aitoffxy(lon,lat,lonx,laty);
			glon[i]->SetPoint(j,lonx,laty);
		}
	}
	for(int i=0;i<nlat;i++)
	{
		glat[i]=new TGraph();
		for(int j=0;j<ndiv;j++)
		{
			double lonx=0,laty=0,lon=-180+360.*j/(ndiv-1),lat=-90+180.*i/(nlat-1);
			//THistPainter::ProjectAitoff2xy(lon,lat,lonx,laty);
			Gal2Aitoffxy(lon,lat,lonx,laty);
			glat[i]->SetPoint(j,lonx,laty);
		}
	}

//Draw Frame and Grids
	TCanvas *c = new TCanvas("c","csky",800,600);
	TH2F *h2fra = new TH2F("h2fra","frame",10,-180.,180,10,-90.,90.);
	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);
	float mg=0.1, mgleg=0.15;
	c->SetTopMargin(mg);
	c->SetBottomMargin(mg);
	c->SetRightMargin(mg);
	c->SetLeftMargin(mg+mgleg);
	c->SetFrameLineColor(kWhite);
	h2fra->Draw("A");
	for(int i=0;i<nlon;i++)
	{
		glon[i]->SetLineColor(kGray);
		glon[i]->SetLineWidth(3);
		glon[i]->Draw("csame");
	}
	for(int i=0;i<nlat;i++)
	{
		glat[i]->SetLineColor(kGray);
		glat[i]->SetLineWidth(3);
		glat[i]->Draw("csame");
	}

//Draw Coodinate Label
	TLatex latex;
	latex.SetTextSize(0.05);
	latex.SetTextFont(132);
	latex.SetTextAlign(32);
	latex.DrawLatexNDC(mg*0.8+mgleg,.5,"+180#circ");
	latex.SetTextAlign(12);
	latex.DrawLatexNDC(1-mg*0.8,.5,"-180#circ");
	latex.SetTextAlign(23);
	latex.DrawLatexNDC(.5+mgleg/2.,mg*0.8,"-90#circ");
	latex.SetTextAlign(21);
	latex.DrawLatexNDC(.5+mgleg/2.,1-mg*0.8,"+90#circ");
	

//Specific Target Source
	TGraph *gtev = new TGraph();
	int ntev=0;
	gtev->SetName("gtev");
	gtev->SetMarkerStyle(29);
	gtev->SetMarkerSize(5);
	gtev->SetMarkerColor(kBlue);
	const std::string targetSrcName = "Crab";

//Get Object from TeVcat List
//copy&paste to copyfromtevcat.txt from tevcat selecting "Name" "Ra" "Dec" "Type"
//cat copyfromtevcat.txt | sed -e 's/\t/\ \,\ /g' -e 's/$/\ \,/' > copyfromtevcatedit.txt
	const char *file = "copyfromtevcatedit.txt";
	ifstream ifst(file);
	if(ifst.fail())
	{
		std::cout << "FAILURE" << std::endl;
		std::cout << "There does not exist the file copyfromtevcatedit.txt, which should contain the TeVCat table." << std::endl;
		std::cout << "copy&paste to copyfromtevcat.txt from TeVCat selecting \"Name\" \"Ra\" \"Dec\" \"Type\"" << std::endl;
		std::cout << "$>cat copyfromtevcat.txt | sed -e 's/\\t/\\ \\,\\ /g' -e 's/$/\\ \\,/' > copyfromtevcatedit.txt" << std::endl;
		std::cout << "Then the content of copyfromtevcatedit.txt should be like this:" << std::endl;
		std::cout << "CTA 1 , 00 06 26 , +72 59 01.0 , PWN ,\n\
SHBL J001355.9-185406 , 00 13 52.0 , -18 53 29 , HBL ,\n\
Tycho , 00 25 27 , +64 10 50 , Shell ,\n\
KUV 00311-1938 , 00 33 34.2 , -19 21 33 , HBL ,\n\
1ES 0033+595 , 00 35 16.8 , +59 47 24.0 , HBL ,\n\
NGC 253 , 00 47 34.3 , -25 17 22.6 , Starburst ,\n\
S2 0109+22 , 01 12 05.8 , +22 44 39 , IBL ,\n\
RGB J0136+391 , 01 36 32.5 , +39 06 00 , HBL ,\n\
RGB J0152+017 , 01 52 33.5 , +01 46 40.3 , HBL , \n\
..." << std::endl;

		return -1;
	}

	std::string ss;
	std::string sname;
	std::string stypes[50];
	TGraph *gtypes[50];
	int ntypes=0;
	int np[50]={0};

	int ncol=0;
	double rh,rm,rs, dd,dm,ds, rdeg,ddeg, rx,dy;
	while(!ifst.eof())
	{
		string s;
		if(ncol==0||ncol==3)
			ifst >> s;
		else if(ncol==1)
		{
			ifst >> rh >> rm >> rs >> s;
			rdeg=hms2deg(rh,rm,rs);
		}
		else if(ncol==2)
		{
			ifst >> dd >> dm >> ds >> s;
			ddeg=dms2deg(dd,dm,ds);
		}

		if(!ifst.good()) break;


		//std::cout << s << std::endl;
		if(s==",")
		{
			if(ncol==0)
			{
				std::cout << "Name " << ss << std::endl;
				sname=ss;
			}
			else if(ncol==1)
				std::cout << "Ra " << rh << "h" << rm << "m" << rs << "s" << std::endl;
			else if(ncol==2)
				std::cout << "Dec " << dd << "d" << dm << "m" << ds << "s" << std::endl;
			else if(ncol==3)
			{
				int typ=-1;
				for(int i=0;i<ntypes;i++)
				{
					if(ss==stypes[i])
						typ=i;
				}

				//if(ss!=stypes[ntypes])
				if(typ==-1)
				{
					np[ntypes]=0;
					stypes[ntypes]=ss;
					gtypes[ntypes]=new TGraph();
					gtypes[ntypes]->SetName(ss.c_str());
					typ=ntypes;
					ntypes++;
				}

				double tmlon,tmlat;
				cel2gal(rdeg,ddeg,tmlon,tmlat);
				//Gal2Aitoffxy(rdeg,ddeg,rx,dy); //warning: this is celestial coordinate
				Gal2Aitoffxy(tmlon,tmlat,rx,dy);

				gtypes[typ]->SetPoint(np[typ],rx,dy);
				np[typ]++;

#ifdef MARK_TARGET_SOURCE
				// mark your specific target source
				if(sname==targetSrcName)
				{
					gtev->SetPoint(ntev,rx,dy);
					ntev++;
					std::cout << "my source found" << std::endl;
				}
#endif

				latex.SetTextSize(0.02);
				latex.SetTextFont(132);
				latex.SetTextAlign(13);
				latex.SetTextColor(kBlack);
				if(abs(tmlat)>2)
				latex.DrawLatex(rx,dy,sname.c_str());

				std::cout << "Type " << ss << std::endl;
			}
			ss = "";
			ncol = ncol==3 ? 0 : ncol+1;
		}
		else
		{
			if(ncol==0||ncol==3)
				ss+=s;
		}
	}
	ifst.close();

//Legend
	TLegend * leg = new TLegend(0.01,0.05,0.30,0.75);
	for(int i=0;i<ntypes;i++)
	{
		gtypes[i]->SetMarkerStyle(24+i/9);
		gtypes[i]->SetMarkerColor(i%9+1);
		gtypes[i]->Draw("psame");
		leg->AddEntry(gtypes[i],stypes[i].c_str(),"p");
		if(stypes[i]=="HBL"||stypes[i]=="IBL"||stypes[i]=="LBL"||stypes[i]=="FSRQ")
		{
			gtypes[i]->SetMarkerStyle(gtypes[i]->GetMarkerStyle()-4);
			//gtypes[i]->SetMarkerColor(kRed);
		}
	}
#ifdef MARK_TARGET_SOURCE
	gtev->Draw("psame");
	leg->AddEntry(gtev,"","");
	leg->AddEntry(gtev,targetSrcName.c_str(),"p");
#endif

	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	leg->Draw("same");

//Output Files
	c->SaveAs("mytevsky.png");
	c->SaveAs("mytevsky.pdf");


	return 0;
}

