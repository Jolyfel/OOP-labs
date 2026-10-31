#pragma once
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>

struct BugReport {
    std::string element;
    std::string type;
    int count;
};

class BugTracker {
    std::multimap<std::string, BugReport> reports_;
    std::map<std::string, int> bugs_by_tester_;
    std::set<std::string> ui_elements_;
    std::multiset<int> report_counts_;
    std::unordered_map<std::string, int> bugs_by_element_;
    std::unordered_set<std::string> bug_types_;
    std::unordered_multimap<std::string, std::string> testers_by_element_;
    std::unordered_multiset<std::string> type_log_;

    void rebuild();
public:
    BugTracker();
    bool add_report(const std::string& tester, const std::string& element,
        const std::string& type, int count);
    bool remove_tester(const std::string& tester);
    void print_ranking(std::ostream& os) const;
    void print_tester(std::ostream& os, const std::string& tester) const;
    void print_element(std::ostream& os, const std::string& element) const;
    void print_containers(std::ostream& os) const;
    bool save_to_file(const std::string& file) const;
};
