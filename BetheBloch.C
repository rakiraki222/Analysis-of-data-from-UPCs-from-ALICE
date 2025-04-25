// run .x BetheBloch.C(filepaths)

double pz_calc(double pT, double eta) {
       double pz = pT * sinh(eta);
       return pz;
       }

double px_calc(double pT, double phi) {
       double px = pT * cos(phi);
       return px;
       }

double py_calc(double pT, double phi) {
       double py = pT * sin(phi);
       return py;
       }
       
double calc_abs_momentum(double pT, double phi, double eta) {
    double p = sqrt( px_calc(pT,phi)*px_calc(pT,phi) + py_calc(pT, phi)*py_calc(pT,phi) + pz_calc(pT, eta)*pz_calc(pT, eta) );
    return p;
    }

void BetheBloch(const std::vector<std::string>& filepaths) {
    TFile *outfile = new TFile("bethebloch.root", "RECREATE");

    TH2F *dedx_p = new TH2F("dedx_p", "dE/dx vs momentum", 300, 0.0, 3.0, 300, 0.0, 200);

    for (const auto& filepath : filepaths) {
        TFile *inputfile = TFile::Open(filepath.c_str());
        if (!inputfile || inputfile->IsZombie()) {
            std::cerr << "Error: cannot open file " << filepath << "!" << std::endl;
            continue;
        } else {
            std::cout << "File " << filepath << " opened successfully" << std::endl;
        }

        TList *keys = inputfile->GetListOfKeys();
        if (!keys) {
            std::cerr << "Failed to get the list of keys in file " << filepath << std::endl;
            inputfile->Close();
            delete inputfile;
            continue;
        }

        TIter next(keys);
        TKey *key;
        while ((key = (TKey *)next())) {
            if (strcmp(key->GetClassName(), "TDirectoryFile") == 0) {
                TDirectoryFile *dir = (TDirectoryFile *)key->ReadObj();
                auto treeReducedTrack = dir->Get<TTree>("O2reducedtrack");
                if (!treeReducedTrack) {
                    std::cerr << "Error: TTree 'O2reducedtrack' not found in directory " << dir->GetName() << std::endl;
                    continue;
                }
                auto treeBarrelPID = dir->Get<TTree>("O2rtbarrelpid");
                if(!treeBarrelPID) {
                    std::cerr << "Error: TTree 'O2rtbarrelpid' not found in directory" << dir->GetName() << std::endl;
                    continue;
                }
                auto treeRtBarrel = dir->Get<TTree>("O2rtbarrel");
                if(!treeRtBarrel) {
                    std::cerr << "Error: TTree 'O2rtbarrel' not found in directory" << dir->GetName() << std::endl;
                    continue;
                }

                float pT, phiangle, beta, eta, dEdx;
                treeReducedTrack->SetBranchAddress("fPt", &pT);
                treeReducedTrack->SetBranchAddress("fPhi", &phiangle);
                treeReducedTrack->SetBranchAddress("fEta", &eta);
                treeBarrelPID->SetBranchAddress("fBeta", &beta);
                treeBarrelPID->SetBranchAddress("fTPCSignal", &dEdx);
                unsigned char findable;
                treeRtBarrel->SetBranchAddress("fTPCNClsFindable", &findable);

                for (int iEntry = 0; iEntry < treeReducedTrack->GetEntries(); ++iEntry) {
                    treeReducedTrack->GetEntry(iEntry);
                    treeBarrelPID->GetEntry(iEntry);
                    treeRtBarrel->GetEntry(iEntry);

                    if (std::abs(std::tan(phiangle)) > 1e-6) { // Avoid division by zero
                        if (findable > 100) {
                           dedx_p->Fill(calc_abs_momentum(pT, phiangle, eta), dEdx);
                           }
                    }
                }
            }
        }
        inputfile->Close();
        delete inputfile;
    }

    outfile->Write();
    outfile->Close();
    delete outfile;
    std::cout << "Analysis complete. Results saved to bethebloch.root" << std::endl;
}

std::vector<std::string> filepaths = {
        "/home/ubuntu/Desktop/AO2D.root",
        "/home/ubuntu/Desktop/AO2D_798.root",
        "/home/ubuntu/Desktop/AO2D_797.root",
        "/home/ubuntu/Desktop/AO2D_796.root",
        "/home/ubuntu/Desktop/AO2D_795.root",
        "/home/ubuntu/Desktop/AO2D_794.root",
        "/home/ubuntu/Desktop/AO2D_793.root",
        "/home/ubuntu/Desktop/AO2D_792.root",
        "/home/ubuntu/Desktop/AO2D_791.root",
        "/home/ubuntu/Desktop/AO2D_790.root"
    };
