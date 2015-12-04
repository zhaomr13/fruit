// Code to Fit the mass of Psi2S
// Author : zhaomr
// Date   : June 4 2015
// Input  : dirData
// Output : dirMass     : fit parameters
//          dirMassPlot : plot
//          dirMassFile : root file

#include <fstream>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <particles.h>
#include <directory.h>
#include <information.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <RooFit.h>
#include <RooPlot.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooAddPdf.h>
#include <RooCBShape.h>
#include <RooExponential.h>
#include <RooStats/SPlot.h>
#include <string>
#include <vector>

using namespace std;
using namespace RooFit;

const double inf = 1e10;

int main()
{
    DataBase data("directory/dirdata");
    // Create DecayTree with the data after reconstruction

    TFile inputFile( data.getAbsDir("selected").c_str() );
    TTree * datatree = (TTree*)inputFile.Get("DecayTree");

    // File !!
    TFile * file = new TFile( data.getAbsDir("massfit").c_str(), "recreate");

    // Useful Variables
    RooRealVar psiMass("psi_MM", "Mass(#mu^{+}#mu^{-})", pdgMassPsi2S/MeV-150, pdgMassPsi2S/MeV+150);
    RooRealVar psiPT("psi_PT", "PT", 0, inf);
    RooRealVar psiY("psi_Y",  "Y ", 0, inf);
    RooRealVar tz("tz", "Tz", -5, 10);
    RooRealVar tzError("tzError", "TzError", 0, 0.3);

    // Data Set to Save the Fit Variables
    RooDataSet dataSet("dataSet", "data set to save the useful variable after selection cut",
            datatree,
            RooArgSet(psiMass, psiPT, psiY, tz, tzError)
            );

    Double_t N = dataSet.numEntries();
    // ----------------  Fit Function # with parameters to adjust --------------------
    // * Correlation of alpha and sigma
    RooRealVar a("a", "a", -0.00018);
    RooRealVar b("b", "b", 0.011);
    RooRealVar c("c", "c", 1.975);
    // Crystal Ball Function parameters
    RooRealVar mean("mean", "Mass_(#psi(2S))", pdgMassPsi2S/MeV, pdgMassPsi2S/MeV - 10, pdgMassPsi2S/MeV + 10, "MeV/c^{2}");
    RooRealVar sigma("sigma", "#sigma_{mass}^{CB}", 13, 5, 20, "MeV/c^{2}");
    RooFormulaVar alpha("alpha", "alpha", "a*sigma*sigma + b*sigma + c", RooArgSet(a, b, c, sigma ) );
    RooRealVar n("n", "n", 1.0);
    // Crystal Ball Function
    RooCBShape signal("signalMass", "signalMass", psiMass, mean, sigma, alpha, n);
    // ------------------------------------------------------
    // Background Function parameters
    RooRealVar p0("p0", "p0", 0.0, -2.0, 2.0, "MeV/c^{2}");
    // Background Function
    RooExponential background("background", "background", psiMass, p0);
    // ------------------------------------------------------
    // number of signal and background
    RooRealVar Nsig("Nsig", "yield of signal", N * 0.8, 0, N);
    RooRealVar Nbkg("Nbkg", "yield of background", N * 0.2, 0, N);
    // signal + background
    RooAddPdf massPDF("massPDF", "signal and background",
            RooArgList(signal, background), RooArgList(Nsig, Nbkg));

    // Fitting
    RooFitResult *fitresult = massPDF.fitTo(dataSet, Minos(kTRUE), Strategy(2), NumCPU(2), Save(true) );

    // Calculae Splot
    dataSet.Write();
    RooStats::SPlot* splot = new RooStats::SPlot("splot", "splot", dataSet, &massPDF, RooArgList(Nsig, Nbkg));

    TTree *result = datatree->CloneTree(0);
    double sig_sw;
    double bkg_sw;
    result->Branch("sig_sw", &sig_sw, "sig_sw/D");
    result->Branch("bkg_sw", &bkg_sw, "bkg_sw/D");
    for (int i = 0; i < N; i++)
    {
        datatree->GetEntry(i);
        sig_sw = ((RooRealVar*)(dataSet.get(i)->find("Nsig_sw")))->getVal();
        bkg_sw = ((RooRealVar*)(dataSet.get(i)->find("Nbkg_sw")))->getVal();
        result->Fill();
    }
    result->Write();


    // Output
    DataBase resultPath("directory/dirmass");
    ofstream fout(resultPath.getDir("fits"));
    vector<string> list;

    fout.precision(4);
    fout << "# fit result (parameters + error)" << endl;

    vector< RooRealVar > Ref;
    Ref.push_back(mean);
    Ref.push_back(sigma);
    Ref.push_back(a);
    Ref.push_back(b);
    Ref.push_back(c);
    Ref.push_back(n);
    Ref.push_back(p0);
    Ref.push_back(Nsig);
    Ref.push_back(Nbkg);

    for (int var = 0; var < Ref.size(); var++)
    {
        fout << Ref[var].GetName() << " = " << Ref[var].getVal() << endl;
        fout << Ref[var].GetName() << "Error = " << Ref[var].getError() << endl;
    }

    fout << "freedom = " << 5 << "" << endl;


    list.push_back("mean");
    list.push_back("sigma");
    list.push_back("p0");
    list.push_back("Nsig");
    list.push_back("Nbkg");

    fout << "# correlation : ";
    for (int i = 0; i < list.size(); i++) fout << list[i] << " ";
    fout << endl;
    fout << "correlation = [" << endl;
    for (int i = 0; i < list.size(); i++)
    {
        fout << "[";
        for (int j = 0; j < list.size(); j++) fout << fitresult->correlation(list[i].c_str(), list[j].c_str()) << ",";
        fout << "]," << endl;
    }
    fout << "]" << endl;
    fout.close();

    file->Close();

    return 0;
}

