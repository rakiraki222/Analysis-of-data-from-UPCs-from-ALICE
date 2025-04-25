void bb_bands2() {
    TFile *outfile = TFile::Open("bethebloch.root");
    TH2F *dedx_p = (TH2F*)outfile->Get("dedx_p");
    if (!dedx_p) {
        std::cerr << "Error: Histogram 'dedx_p' not found!" << std::endl;
        return;
    }
    
    TCanvas *c1 = new TCanvas("c1", "Particle Identification with Bethe-Bloch", 800, 600);
    c1->SetLogz();
    dedx_p->Draw("colz");
    
    TF1 *f1 = new TF1("f1", "1/(0.17*pow(x-0.25,2))+65", 0.25, 1.5);
    f1->SetLineColor(kBlack);
    f1->SetLineWidth(3);
    f1->SetLineStyle(10);
    f1->Draw("same");
    
    TF1 *f2 = new TF1("f2", "1/(0.5*pow(x-0.16,2))+60", 0.16, 1.5);
    f2->SetLineColor(kBlack);
    f2->SetLineWidth(3);
    f2->SetLineStyle(10);
    f2->Draw("same");
    
    TF1 *f3 = new TF1("f3", "1/(5*pow(x-0.1,2))+60", 0.1, 1.5);
    f3->SetLineColor(kBlack);
    f3->SetLineWidth(3);
    f3->SetLineStyle(10);
    f3->Draw("same");
    
    TF1 *f4 = new TF1("f4", "1/(0.08*pow(x-0.4,2))+78", 0.5, 1.5);
    f4->SetLineColor(kBlack);
    f4->SetLineWidth(3);
    f4->SetLineStyle(10);
    f4->Draw("same");


  auto makeBB = [](const char* name, Color_t color, double p0, double p1, double p2, double xmin) {
  TF1 *f = new TF1("f", "1/([0]*pow(x-[1],2)) + [2]", xmin, 1.5);
        f->SetParameters(p0, p1, p2);
        gStyle->SetOptStat(0); 
        f->SetLineColor(color);
        f->SetLineWidth(2.5);
        f->Draw("same");
        return f;
    };

    TF1 *f_pion = makeBB("f_pion", kRed,   0.5, 0, 60, 0.15);
    TF1 *f_kaon = makeBB("f_kaon", kBlue,  0.4, 0.17, 65, 0.17);
    TF1 *f_proton = makeBB("f_proton", kGreen+2, 0.08, 0.25, 65, 0.25);

    TLegend *leg = new TLegend(0.7, 0.7, 0.9, 0.9);
    leg->AddEntry(f_pion, "Pions", "l");
    leg->AddEntry(f_kaon, "Kaons", "l");
    leg->AddEntry(f_proton, "Protons", "l");
    leg->Draw();

    c1->Update();
    
}
