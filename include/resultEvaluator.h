#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <boost/filesystem/operations.hpp>
#include <math.h>
#include <boost/tuple/tuple.hpp>
#include "gnuplot-iostream.h"


namespace resultEvaluator{
	class Evaluator{
		public:
            //! \brief Constructor
            //! \brief resultFolder Path to the folder containing the Ground Truth Evaluator Result files
            Evaluator(std::string resultsFolder,std::string objName);

            //! \brief EvaluatorInitializer
            void EvaluatorInit(std::string pathToResults);

            //! \brief read contents of the folder for scores
            bool loadResults(std::vector< std::vector<double> > results);

            //! \brief sort the contents according to L2 error score
            bool initsortResults(std::vector< std::vector<double> > toBeSortedResults, int numberOfMatches, std::vector<std::vector<double> > &finalResults);

            //! \brief comupute table according to Rotation threshold using the sorted vector per frame
            bool computeRotation(std::vector< std::vector<double> > finalResults, std::vector<double> errThreshold,std::vector< std::vector< double> > &rotResults);

            //! \brief check thresholds and compute results
            //! \brief give the %age of detections
            //! \brief give the mean rotation and translation errors
            bool checkResults(std::vector< std::vector<double> > SortedResults, std::vector<std::vector<double> > &finalResults);

            //! \brief build the table
            bool computeTable( std::vector<int> errThreshold,std::vector<std::vector<double> > finalResults);

            //! \brief save results to file
            void saveResults(std::string filePathDetection, std::string filePathRotation, std::string filePathRotErrors, std::string tableName);

		private:
            std::string m_resultsFolder;
            std::string m_pathToResultsFile;
            std::string m_objName;
            int m_FrameCount;
            std::vector<double> m_errorThreshold;
            std::vector< std::vector<double> > m_finalResults;
            std::vector< std::vector<double> > m_rotResults;
            std::vector<double> m_meanTransError;
        };
}
		
