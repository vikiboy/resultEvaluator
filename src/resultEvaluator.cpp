#include "resultEvaluator.h"

const double RAD2DEG = 180.0/M_PI;

namespace resultEvaluator{

    Evaluator::Evaluator(std::string resultsFolder, std::string objName){
        m_resultsFolder=resultsFolder;
        m_objName=objName;
        if(!boost::filesystem::exists(m_resultsFolder)){
            std::cout<<"The folder doesn't exist"<<std::endl;
        }

        std::string m_pathToresultsFile_detection = resultsFolder + "results-" + m_objName + ".txt";
        if(!boost::filesystem::exists(m_pathToresultsFile_detection)){
            std::cout<<"The file doesn't exist"<<std::endl;
        }

        this->EvaluatorInit(m_pathToresultsFile_detection);
    }

    void Evaluator::EvaluatorInit(std::string pathToResults){
        std::vector<std::vector<double> > results;
        std::vector<double> frameId;
        std::vector<double> matchId;
        std::vector<double> errL2;
        std::vector<double> errRx;
        std::vector<double> errRy;
        std::vector<double> errRz;
        std::vector<double> errRT;
        std::ifstream evaluationReader(pathToResults.c_str());
        double count=0;
        int n_Id_last=0,n_Id_new=0,n_Match=0;
        double n_ErrL2,n_ErrRx,n_ErrRy,n_ErrRz,n_ErrId,n_ErrRT;
        if(evaluationReader.is_open()){
            while (!evaluationReader.eof()){
                evaluationReader>>n_Id_new>>n_Match>>n_ErrL2>>n_ErrRx>>n_ErrRy>>n_ErrRz>>n_ErrId>>n_ErrRT;
                if(n_Id_last==0){
                    n_Id_last=n_Id_new;
                    count=1;
                }
                if(n_Id_new!=n_Id_last){
                    ++count;
                    n_Id_last=n_Id_new;
                }
                frameId.push_back(count);
                matchId.push_back(n_Match);
                errL2.push_back(n_ErrL2);
                errRx.push_back(n_ErrRx);
                errRy.push_back(n_ErrRy);
                errRz.push_back(n_ErrRz);
                errRT.push_back(n_ErrRT);
            }
        }
        results.push_back(frameId);
        results.push_back(matchId);
        results.push_back(errL2);
        results.push_back(errRx);
        results.push_back(errRy);
        results.push_back(errRz);
        results.push_back(errRT);
        m_FrameCount=count;

        if(this->loadResults(results)){
            std::cout<<"Loaded Results Successfully"<<std::endl;
        }
        else{
            std::cout<<"Failed to laod results"<<std::endl;
        }
    }

    bool Evaluator::loadResults(std::vector<std::vector<double> > results){
        std::vector< std::vector<double> > sortTemp;
        std::vector<double> Temp;
        std::vector< std::vector<double> > FinalResults;
        std::vector< std::vector<double> > DetectionResults;
        FinalResults.clear();
        int count_Matches=0,finalCount;
        if(results[0].size()!=0){
            for(int i=1;i<=m_FrameCount;i++){
                for(int k=0;k<7;k++){
                    for(int j=1;j<results[0].size();j++){
                        if(results[0][j-1]==i){
                            ++count_Matches;
                            Temp.push_back(results[k][j-1]);
                        }
                    }
                    sortTemp.push_back(Temp);
                    Temp.clear();
                    finalCount=count_Matches;
                    count_Matches=0;
                }
                if(this->initsortResults(sortTemp,finalCount,FinalResults)){
                }
                sortTemp.clear();
            }
            std::cout<<"Sorted results of all frames"<<std::endl;
            std::cout<<"\n"<<std::endl;
            if(this->checkResults(FinalResults,DetectionResults)){
            }
            return true;
        }
        else{
            return false;
        }
    }

    bool Evaluator::initsortResults(std::vector<std::vector<double> > toBeSortedResults,int numberOfMatches, std::vector<std::vector<double> > &finalResults){
        std::vector<double> sortMatchID;
        std::vector<double> sortErrL2;
        std::vector<double> sortErrRx;
        std::vector<double> sortErrRy;
        std::vector<double> sortErrRz;
        std::vector<double> sortErrRT;

        int tempL2=0,tempId=0;
        for(int i=0;i<numberOfMatches;i++){
            sortMatchID.push_back(toBeSortedResults[1][i]);
            sortErrL2.push_back(toBeSortedResults[2][i]);
            sortErrRx.push_back(toBeSortedResults[3][i]);
            sortErrRy.push_back(toBeSortedResults[4][i]);
            sortErrRz.push_back(toBeSortedResults[5][i]);
            sortErrRT.push_back(toBeSortedResults[6][i]);
        }
        for(int j=0;j<numberOfMatches;j++){
            for(int i=0;i<numberOfMatches;i++){
                if(sortErrL2[i]<sortErrL2[i+1]){
                    continue;
                }
                else{
                    if(i!=numberOfMatches-1){
                        std::iter_swap(sortMatchID.begin()+(i),sortMatchID.begin()+(i+1));
                        std::iter_swap(sortErrL2.begin()+(i),sortErrL2.begin()+(i+1));
                        std::iter_swap(sortErrRx.begin()+(i),sortErrRx.begin()+(i+1));
                        std::iter_swap(sortErrRy.begin()+(i),sortErrRy.begin()+(i+1));
                        std::iter_swap(sortErrRz.begin()+(i),sortErrRz.begin()+(i+1));
                        std::iter_swap(sortErrRT.begin()+(i),sortErrRT.begin()+(i+1));
                    }
                }
            }
        }

        finalResults.push_back(toBeSortedResults[0]);       // 0,7,14,...
        finalResults.push_back(sortMatchID);                //1,8,15,...7k+1
        finalResults.push_back(sortErrL2);                  //2,9,16,...7k+2
        finalResults.push_back(sortErrRx);                  //3,10,17,...7k+3
        finalResults.push_back(sortErrRy);                  //4,11,18,...7k+4
        finalResults.push_back(sortErrRz);                  //5,12,19,...7k+5
        finalResults.push_back(sortErrRT);                  //6,13,20,...7k+6
        return true;
    }

    bool Evaluator::checkResults(std::vector<std::vector<double> > SortedResults, std::vector<std::vector<double> > &finalResults){
        std::vector<double> results;
        int flags_zero[]={0,0,0,0,0},flags_best[]={0,0,0,0,0},count_DetectionsZero[]={0,0,0,0,0},count_DetectionsBest[]={0,0,0,0,0};
        std::vector<double> percentDetections_zero,percentDetections_best;
        std::vector<double> ErrRx,ErrRy,ErrRz,ErrRT ;
        std::vector<double> mean_ErrRx,mean_ErrRy,mean_ErrRz,mean_ErrRT ;


        static const int error[] = {200,300,400,500,600};
        std::vector<int> errThreshold (error, error + sizeof(error) / sizeof(error[0]) );

        double mean_Rx=0,mean_Ry=0,mean_Rz=0,mean_RT=0;

        for(int i=0;i<m_FrameCount;i++){
            for(int j=0;j<SortedResults[7*i].size();j++){
                for(int k=0;k<errThreshold.size();k++){
                        if(SortedResults[7*i+2][j]<=errThreshold[k]){
                                    flags_best[k]++;
                            }

                        if((SortedResults[7*i+1][j]==0)&&(SortedResults[7*i+2][j]<=errThreshold[k])){
                                    flags_zero[k]++;
                            }
                        }
                    }

                for(int k=0;k<errThreshold.size();k++){
                    if(flags_best[k]>0){
                        count_DetectionsBest[k]++;
                        ErrRx.push_back(SortedResults[7*i+3][0]);
                        ErrRy.push_back(SortedResults[7*i+4][0]);
                        ErrRz.push_back(SortedResults[7*i+5][0]);
                        ErrRT.push_back(SortedResults[7*i+6][0]);
                    }
                    else{
                        ErrRx.push_back(0);
                        ErrRy.push_back(0);
                        ErrRz.push_back(0);
                        ErrRT.push_back(0);
                    }

                    if(flags_zero[k]>0){
                        count_DetectionsZero[k]++;
                    }

                    flags_best[k]=0;
                    flags_zero[k]=0;
                }
        }

        for(int k=0;k<errThreshold.size();k++){
            mean_Rx=0;
            mean_Ry=0;
            mean_Rz=0;
            mean_RT=0;
            for(int i=0;i<count_DetectionsBest[k];i++){
                 mean_Rx+=ErrRx[errThreshold.size()*i+k];
                 mean_Ry+=ErrRy[errThreshold.size()*i+k];
                 mean_Rz+=ErrRz[errThreshold.size()*i+k];
                 mean_RT+=ErrRT[errThreshold.size()*i+k];
            }

            mean_ErrRx.push_back(RAD2DEG*mean_Rx/count_DetectionsBest[k]);
            mean_ErrRy.push_back(RAD2DEG*mean_Ry/count_DetectionsBest[k]);
            mean_ErrRz.push_back(RAD2DEG*mean_Rz/count_DetectionsBest[k]);
            mean_ErrRT.push_back(mean_RT*0.10/count_DetectionsBest[k]);
            percentDetections_best.push_back((float)count_DetectionsBest[k]*100/((float)m_FrameCount));
            percentDetections_zero.push_back((float)count_DetectionsZero[k]*100/((float)m_FrameCount));
        }

        for(int k=0;k<errThreshold.size();k++){
    //        std::cout<<"Error Threshold :"<<errThreshold[k]<<std::endl;
    //        std::cout<<"Detection Percentage (Top 20) : "<<percentDetections_best[k]<<"\t\t";
    //        std::cout<<"Detection Percentage (Highest Similarity Score): "<<percentDetections_zero[k]<<std::endl;
    //        std::cout<<"Rotation Error in X : "<<mean_ErrRx[k]<<"\t\t Y : "<<mean_ErrRy[k]<<"\t\t Z : "<<mean_ErrRz[k]<<std::endl;
    //        std::cout<<"Translation Error : "<<mean_ErrRT[k]<<std::endl;
    //        std::cout<<"\n"<<std::endl;
            results.push_back(errThreshold[k]);
            results.push_back(percentDetections_zero[k]);
            results.push_back(percentDetections_best[k]);
            results.push_back(mean_ErrRx[k]);
            results.push_back(mean_ErrRy[k]);
            results.push_back(mean_ErrRz[k]);
            results.push_back(mean_ErrRT[k]);
            finalResults.push_back(results);
            results.clear();
        }
        if(this->computeTable(errThreshold,finalResults)){
            m_finalResults=finalResults;
            return true;
        }
    }

    bool Evaluator::computeTable(std::vector<int> errThreshold, std::vector<std::vector<double> > finalResults){

        std::cout<<"Error Threshold"<<"\t\t"<<"%age Detections (Highest Similarity Score)"<<"\t\t"<<"%age Detections (Top 20)"<<"\n"<<std::endl;
        for(int k=0;k<errThreshold.size();k++){
            std::cout<<finalResults[k][0]<<"\t\t"<<finalResults[k][1]<<"\t\t"<<finalResults[k][2]<<"\n"<<std::endl;
        }

        std::cout<<"\t\tRotation Error"<<"\t\t"<<"Translation Error\n"<<std::endl;
        std::cout<<"X :"<<"\t"<<"\tY :"<<"\t"<<"\tZ :"<<std::endl;
        for(int k=0;k<errThreshold.size();k++){
            std::cout<<finalResults[k][3]<<"\t\t"<<finalResults[k][4]<<"\t\t"<<finalResults[k][5]<<"\t\t"<<finalResults[k][6]<<"\n"<<std::endl;
        }

        return true;
    }

    bool Evaluator::saveResults(std::string filePathDetection,std::string filePathRotation, std::string tableName){
        std::ofstream resultsFile_detection;
        resultsFile_detection.open(filePathDetection.c_str(),std::ios::out);
        resultsFile_detection << "\t\t\t******"<<tableName<<"******\n"<<std::endl;
        resultsFile_detection<<"Error Threshold"<<"\t\t"<<"%age Detections (Highest Similarity Score)"<<"\t\t"<<"%age Detections (Top 20)"<<"\n"<<std::endl;
        for(int k=0;k<m_finalResults.size();k++){
            resultsFile_detection<<m_finalResults[k][0]<<"\t\t"<<m_finalResults[k][1]<<"\t\t"<<m_finalResults[k][2]<<"\n"<<std::endl;
        }

        std::ofstream resultsFile_rotation;
        resultsFile_rotation.open(filePathRotation.c_str(),std::ios::out);
        resultsFile_rotation << "\t\t\t******"<<tableName<<"******\n"<<std::endl;
        resultsFile_rotation<<"\t\tRotation Error"<<"\t\t"<<"Translation Error\n"<<std::endl;
        resultsFile_rotation<<"X :"<<"\t\t"<<"Y :"<<"\t\t"<<"Z :"<<std::endl;
        for(int k=0;k<m_finalResults.size();k++){
            resultsFile_rotation<<m_finalResults[k][3]<<"\t\t"<<m_finalResults[k][4]<<"\t\t"<<m_finalResults[k][5]<<"\t\t"<<m_finalResults[k][6]<<"\n"<<std::endl;
        }
    }


}


