#include "resultEvaluator.h"

int main(int argc,char* argv[])
{
    if(argc!=3){
        std::cout<<"****** RESULTS EVALUATOR ******\n usage:"<<argv[0]<<" [path to Protobuf file][Object Name]"<<std::endl;
        return -1;
    }
    std::string pathName = std::string(argv[1]);
    std::string objName = std::string(argv[2]);
    resultEvaluator::Evaluator evaluator(pathName,objName);

    std::string folderPath,pathToDetectionResults,pathToRotTransResults;
    folderPath=pathName+"/"+objName+"-evaluationResults/";
    if(!boost::filesystem::exists(folderPath)){
        boost::filesystem::path dir(folderPath);
        boost::filesystem::create_directories(dir);
    }
    pathToDetectionResults=folderPath+"detectionResults.txt";
    pathToRotTransResults=folderPath+"RotTransResults.txt";
    evaluator.saveResults(pathToDetectionResults,pathToRotTransResults,objName);
    return 0;
}

