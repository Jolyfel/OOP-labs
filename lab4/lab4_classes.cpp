#include <fstream>
#include <functional>
#include "l4_class_header.h"

using namespace std;

static string errors_word(int n) {
    int last_two = n % 100;
    int last = n % 10;
    if (last_two >= 11 && last_two <= 14) return "ошибок";
    if (last == 1) return "ошибку";
    if (last >= 2 && last <= 4) return "ошибки";
    return "ошибок";
}

BugTracker::BugTracker() {
    add_report("Angel", "Button", "layout", 6);
    add_report("Angel", "TextField", "crash", 4);
    add_report("Dia", "Label", "color", 6);
    add_report("Max", "Button", "crash", 3);
}

void BugTracker::rebuild() {
    bugs_by_tester_.clear();
    ui_elements_.clear();
    report_counts_.clear();
    bugs_by_element_.clear();
    bug_types_.clear();
    testers_by_element_.clear();
    type_log_.clear();
    for (const auto& r : reports_) {
        const string& tester = r.first;
        const BugReport& b = r.second;
        bugs_by_tester_[tester] += b.count;
        ui_elements_.insert(b.element);
        report_counts_.insert(b.count);
        bugs_by_element_[b.element] += b.count;
        bug_types_.insert(b.type);
        testers_by_element_.insert({ b.element, tester });
        type_log_.insert(b.type);
    }
}

bool BugTracker::add_report(const string& tester, const string& element,
    const string& type, int count) {
    if (tester.empty() || element.empty() || type.empty() || count <= 0) return false;
    reports_.insert({ tester, BugReport{element, type, count} });
    rebuild();
    return true;
}

bool BugTracker::remove_tester(const string& tester) {
    if (reports_.erase(tester) == 0) return false;
    rebuild();
    return true;
}

void BugTracker::print_ranking(ostream& os) const {
    multimap<int, string, greater<int>> ranking;
    for (const auto& p : bugs_by_tester_) ranking.insert({ p.second, p.first });
    if (ranking.empty()) {
        os << "(нет данных)\n";
        return;
    }
    int place = 1;
    for (const auto& p : ranking)
        os << place++ << ". тестировщик " << p.second << " выявил "
        << p.first << " " << errors_word(p.first) << "\n";
}

void BugTracker::print_tester(ostream& os, const string& tester) const {
    auto it = bugs_by_tester_.find(tester);
    if (it == bugs_by_tester_.end()) {
        os << "Тестировщик не найден.\n";
        return;
    }
    os << "тестировщик " << tester << " выявил " << it->second << " "
        << errors_word(it->second) << "\n";
    auto range = reports_.equal_range(tester);
    for (auto i = range.first; i != range.second; ++i)
        os << "  элемент: " << i->second.element << ", тип: " << i->second.type
        << ", ошибок: " << i->second.count << "\n";
}

void BugTracker::print_element(ostream& os, const string& element) const {
    auto it = bugs_by_element_.find(element);
    if (it == bugs_by_element_.end()) {
        os << "UI-элемент не найден.\n";
        return;
    }
    set<string> testers;
    auto range = testers_by_element_.equal_range(element);
    for (auto i = range.first; i != range.second; ++i) testers.insert(i->second);
    os << "UI-элемент " << element << ": всего ошибок " << it->second << ", нашли:";
    for (const auto& t : testers) os << " " << t;
    os << "\n";
}

void BugTracker::print_containers(ostream& os) const {
    os << "\n=== 1. map: тестировщик -> всего ошибок ===\n";
    for (const auto& p : bugs_by_tester_) os << "  " << p.first << " -> " << p.second << "\n";

    os << "\n=== 2. set: UI-элементы ===\n";
    for (const auto& e : ui_elements_) os << "  " << e << "\n";

    os << "\n=== 3. multimap: тестировщик -> (элемент, тип, ошибок) ===\n";
    for (const auto& r : reports_)
        os << "  " << r.first << " -> (" << r.second.element << ", "
        << r.second.type << ", " << r.second.count << ")\n";

    os << "\n=== 4. multiset: количество ошибок в отчётах ===\n";
    for (auto it = report_counts_.begin(); it != report_counts_.end();
        it = report_counts_.upper_bound(*it))
        os << "  " << *it << " (отчётов: " << report_counts_.count(*it) << ")\n";

    os << "\n=== 5. unordered_map: UI-элемент -> всего ошибок ===\n";
    for (const auto& p : bugs_by_element_) os << "  " << p.first << " -> " << p.second << "\n";

    os << "\n=== 6. unordered_set: типы ошибок ===\n";
    for (const auto& t : bug_types_) os << "  " << t << "\n";

    os << "\n=== 7. unordered_multimap: UI-элемент -> тестировщик ===\n";
    for (const auto& p : testers_by_element_) os << "  " << p.first << " -> " << p.second << "\n";

    os << "\n=== 8. unordered_multiset: частота типов ошибок ===\n";
    for (const auto& t : bug_types_)
        os << "  " << t << ": " << type_log_.count(t) << "\n";
}

bool BugTracker::save_to_file(const string& file) const {
    ofstream f(file);
    if (!f) return false;
    f << "=== РЕЙТИНГ ТЕСТИРОВЩИКОВ ===\n";
    print_ranking(f);
    print_containers(f);
    return true;
}