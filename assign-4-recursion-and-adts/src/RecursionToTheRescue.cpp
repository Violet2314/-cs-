#include "RecursionToTheRescue.h"
#include "map.h"
#include <climits>
#include <iostream>
#include "Disasters.h"
using namespace std;

/* * * * Doctors Without Orders * * * */

/**
 * Given a list of doctors and a list of patients, determines whether all the patients can
 * be seen. If so, this function fills in the schedule outparameter with a map from doctors
 * to the set of patients that doctor would see.
 *
 * @param doctors  The list of the doctors available to work.
 * @param patients The list of the patients that need to be seen.
 * @param schedule An outparameter that will be filled in with the schedule, should one exist.
 * @return Whether or not a schedule was found.
 */
bool getall(int now,
            Vector<Doctor>& doctors,
            Vector<Patient>& patients,
            Map<string,Vector<string>> &schedule){
    if(patients.size() == 0) return true;
    if(now == doctors.size()) return false;
    bool cannext = false;
    Vector<Doctor> doctornow = doctors;
    for(int i = 0; i < patients.size(); i++){
        if(doctors[now].hoursFree >= patients[i].hoursNeeded){
            Patient p = patients[i];
            cannext = true;

            schedule[doctors[now].name].add(patients[i].name);
            doctors[now].hoursFree -= patients[i].hoursNeeded;
            patients.remove(i);

            if(getall(now,doctors,patients,schedule)) return true;

            patients.insert(i,p);
            doctors[now].hoursFree += patients[i].hoursNeeded;
            schedule[doctors[now].name].remove(schedule[doctors[now].name].size()-1);
        }
    }
    if(cannext == false){
        if(getall(now+1,doctors,patients,schedule)) return true;
    }
    return false;
}


bool canAllPatientsBeSeen(const Vector<Doctor>& doctors,
                          const Vector<Patient>& patients,
                          Map<string, Set<string>>& schedule) {
    // [TODO: Delete these lines and implement this function!]
    Vector<Doctor> doctornow = doctors;
    Vector<Patient> patientnow = patients;
    Map<string,Vector<string>> schedulenow;
    if(getall(0,doctornow,patientnow,schedulenow)){
        for(int i = 0; i < doctors.size(); i++){
            for(int j = 0; j < schedulenow[doctors[i].name].size(); j++){
                schedule[doctors[i].name].add(schedulenow[doctors[i].name][j]);
                cout << doctors[i].name << " ";
            }
        }
        return true;
    }
    return false;
}

/* * * * Disaster Planning * * * */

/**
 * Given a transportation grid for a country or region, along with the number of cities where disaster
 * supplies can be stockpiled, returns whether it's possible to stockpile disaster supplies in at most
 * the specified number of cities such that each city either has supplies or is connected to a city that
 * does.
 * <p>
 * This function can assume that every city is a key in the transportation map and that roads are
 * bidirectional: if there's a road from City A to City B, then there's a road from City B back to
 * City A as well.
 *
 * @param roadNetwork The underlying transportation network.
 * @param numCities   How many cities you can afford to put supplies in.
 * @param locations   An outparameter filled in with which cities to choose if a solution exists.
 * @return Whether a solution exists.
 */
bool getBeMadeDisasterReady(const Map<string,Set<string>> &roadNetwork,
                            vector<string> &keys,
                            int &numCities,
                            Set<string> &covercity,
                            Set<string>& locations){

    if(locations.size() > numCities) return false;
    if(covercity.size() == keys.size()) return true;

    vector<string> nows;
    for(auto it : keys){
        nows.clear();
        if(!locations.contains(it)){
            if(!covercity.contains(it)){
                nows.push_back(it);
                covercity.add(it);
            }
            for(auto now : roadNetwork[it]){
                if(!covercity.contains(now)){
                    nows.push_back(now);
                    covercity.add(now);
                }
            }
            locations.add(it);

            if(getBeMadeDisasterReady(roadNetwork,keys,numCities,covercity,locations)){
                return true;
            }

            locations.remove(it);
            for(auto now : nows){
                covercity.remove(now);
            }
        }
    }
    return false;
}


bool canBeMadeDisasterReady(const Map<string, Set<string>>& roadNetwork,
                            int numCities,
                            Set<string>& locations) {
    // [TODO: Delete these lines and implement this function!]]
    vector<string> keys;
    Set<string> covercity;
    for(auto it : roadNetwork){
        keys.push_back(it);
    }
    if(getBeMadeDisasterReady(roadNetwork,keys,numCities,covercity,locations)){
        return true;
    }
    return false;
}

/* * * * Winning the Election * * * */

/**
 * Given a list of the states in the election, including their popular and electoral vote
 * totals, and the number of electoral votes needed, as well as the index of the lowest-indexed
 * state to consider, returns information about how few popular votes you'd need in order to
 * win that at least that many electoral votes.
 *
 * @param electoralVotesNeeded the minimum number of electoral votes needed
 * @param states All the states in the election (plus DC, if appropriate)
 * @param minStateIndex the lowest index in the states Vector that should be considered
 */
MinInfo minPopularVoteToGetAtLeast(int electoralVotesNeeded, const Vector<State>& states, int minStateIndex) {
    // [TODO: Delete these lines and implement this function!]
    (void)(electoralVotesNeeded);
    (void)(states);
    (void)(minStateIndex);
    return { 0, {} };
}

/**
 * Given a list of all the states in an election, including their popular and electoral vote
 * totals, returns information about how few popular votes you'd need to win in order to win
 * the presidency.
 *
 * @param states All the states in the election (plus DC, if appropriate)
 * @return Information about how few votes you'd need to win the election.
 */
void getminPopularVoteToWin(int (&vis)[100],
                            int &ans,
                            int &nowpopularVotes,
                            int &nowelectoralVotes,
                            const Vector<State> &states,
                            Vector<State> &now,
                            Vector<State> &anss,
                            int need){
    for(int i = 0; i < states.size(); i++){
        if(vis[i] == 1) continue;
        vis[i] = 1;
        nowpopularVotes += (states[i].popularVotes)/2 + 1;
        nowelectoralVotes += states[i].electoralVotes;
        now.add(states[i]);
        if(nowelectoralVotes < need){
            getminPopularVoteToWin(vis,ans,nowpopularVotes,nowelectoralVotes,states,now,anss,need);
        }else{
            if(nowpopularVotes < ans){
                ans = nowpopularVotes;
                anss.clear();
                anss.addAll(now);
            }
        }
        now.remove(now.size() - 1);
        nowelectoralVotes -= states[i].electoralVotes;
        nowpopularVotes -= (states[i].popularVotes)/2 + 1;
        vis[i] = 0;
    }
}

int getneed(const Vector<State>& states){
    int ans = 0;
    for(auto it : states){
        ans += it.electoralVotes;
    }
    return ans;
}

MinInfo minPopularVoteToWin(const Vector<State>& states) {
    // [TODO: Delete these lines and implement this function!]
    Vector<State> now;
    Vector<State> anss;
    int need = getneed(states);
    need = need/2 + 1;
    int vis[100];
    int ans = 1e9;
    int nowpopularVotes = 0;
    int nowelectoralVotes = 0;
    getminPopularVoteToWin(vis,ans,nowpopularVotes,nowelectoralVotes,states,now,anss,need);
    return { ans, {anss} };
}
