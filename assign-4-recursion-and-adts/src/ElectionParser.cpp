#include "strlib.h"
#include <string>
#include <fstream>
#include "vector.h"
#include "Elections.h"
#include <QDir>
#include <QCoreApplication>
using namespace std;

/* Everything in here is private to the file. */
namespace {
    /* How many lines to read if the data are simplified first. */
    const int kSimplifiedLines = 10;


    /**
     * Given a line from a file of the form
     *    State Name, Postal Code, Electoral Votes, Votes Cast, Population
     * returns a State object constructed from the relevant information.
     *
     * @param line The line of the file to parse.
     * @return A State object constructed from it.
     * @throws ErrorException If the line is malformed.
     */
    State parseState(const string& line) {
        /* Enumerated type corresponding to the different components of
         * a line from the input file.
         */
        enum LineComponents {
            StateName,
            PostalCode,
            ElectoralVotes,
            PopularVotes,
            NumComponents
        };

        auto pieces = stringSplit(line, ",");
        if (pieces.size() != LineComponents::NumComponents) {
            error("Wrong number of entries on this line.");
        }

        return {
            pieces[LineComponents::StateName],
            stringToInteger(pieces[LineComponents::ElectoralVotes]),
            stringToInteger(pieces[LineComponents::PopularVotes])
        };
    }

    /**
     * Parses the first line of an elections file, confirming that it matches the
     * expected year.
     *
     * @param minInfo The information struct to fill in.
     * @param line    The line from the file.
     * @param year    The year in question.
     * @param shouldSimplify Whether to use the simplified or the full data
     * @throws ErrorException If the line isn't valid.
     */
    void parseFirstLine(ElectionTest& info, const string& line, int year, bool shouldSimplify) {
        /* Type representing the pieces of the header that we'd need. */
        enum HeaderLineComponents {
            Year,
            MinPopularVote,
            MinSimplePopularVote,
            NumComponents
        };

        /* Split the line into its components. It should have the same number as
         * a regular line.
         */
        auto pieces = stringSplit(line, ",");
        if (pieces.size() != HeaderLineComponents::NumComponents) {
            error("Wrong number of entries on this line.");
        }

        /* Make sure this is for the right year, just as a sanity check. */
        if (stringToInteger(pieces[HeaderLineComponents::Year]) != year) {
            error("Election data file doesn't match the expected year.");
        }

        /* Pull the expected test result, which depends on whether we're simplifying things. */
        int answerIndex = shouldSimplify? HeaderLineComponents::MinSimplePopularVote : HeaderLineComponents::MinPopularVote;
        info.minPopularVoteNeeded = stringToInteger(pieces[answerIndex]);
    }
}

/**
 * Given an election year, returns a Vector<State> containing all the
 * election data from that year. If the shouldSimplify flag is set, the
 * data set is first reduced down to ten states.
 *
 * @param year The year in question. Valid years are 1828 - 2016.
 * @param shouldSimplify Whether to use the simplified version of the data rather than the full.
 * @return A list of all the states from the election in that year.
 * @throws ErrorException If an error occurs or the year is invalid.
 */
ElectionTest electionDataFrom(int year, bool shouldSimplify) {

    ifstream input;

    string directory = "res/Elections/Data/";

    // figure out correct absolute path to the dictionary, so the demo can run correctly
    string filename = integerToString(year) + ".csv";
    QDir dir(QCoreApplication::applicationDirPath());
    QString absolute_project_file_path = dir.absolutePath();
    string absPath = absolute_project_file_path.toLatin1().data();
    absPath = absPath + "/" + directory + filename;

    /* First, try to open the file using the absolute path name. */
    {
        ifstream fileReader(absPath);
        if (fileReader) {
            input = std::move(fileReader);
        }
    }

    string fullname1 = directory + filename;

    /* Then, try to open the file using the updated relative path name. */
    {
        ifstream fileReader(fullname1);
        if (fileReader) {
            input = std::move(fileReader);
        }
    }

    string fullname2 = directory.substr(directory.find("/") + 1) + filename;

    /* Then, try to open the file using the reduced relative path name (for windows). */
    {
        ifstream fileReader(fullname2);
        if (fileReader) {
            input = std::move(fileReader);
        }
    }

    /* Try to open the file just using the name given. */
    {
        ifstream fileReader(filename);
        if (fileReader) {
            input = std::move(fileReader);
        }
    }

    if (!input) {
        error("Can't open a data file for that year.");
    }

    ElectionTest result;
    result.year = year;

    /* Confirm that the year matches and extract the reference solution. */
    string firstLine;
    getline(input, firstLine);
    parseFirstLine(result, firstLine, year, shouldSimplify);

    /* Parse each line of the file. */
    for (string line; getline(input, line) && !(shouldSimplify && result.allStates.size() == kSimplifiedLines); ) {
        result.allStates += parseState(line);
    }

    /* If when all things are said and done we couldn't read
     * anything, we've encountered an error.
     */
    if (result.allStates.isEmpty()) error("File is missing or empty?");

    return result;
}
