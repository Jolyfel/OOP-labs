#include <algorithm>
#include <fstream>
#include "l3_class_header.h"

using namespace std;

static string pad(const string& s, size_t w) {
    size_t len = 0;
    for (unsigned char c : s) if ((c & 0xC0) != 0x80) ++len;
    return s + string(len < w ? w - len : 0, ' ');
}
static bool has(const string& s, const string& part) { return s.find(part) != string::npos; }
static string type_name(int t) {
    switch (t) { case 1: return "Button"; case 2: return "TextField"; case 3: return "Label"; }
                       return "";
}

Style::Style(const string& name, const string& color, int font_size)
    : name_(name), color_(color), font_size_(font_size) {
}

void Style::set(const string& name, const string& color, int font_size) {
    if (!name.empty()) name_ = name;
    if (!color.empty()) color_ = color;
    if (font_size > 0) font_size_ = font_size;
}

UIElement::UIElement(int id, const string& name, int w, int h, const Style& s)
    : id_(id), name_(name), width_(w), height_(h), style_(s) {
}

void UIElement::set_name(const string& n) { if (!n.empty()) name_ = n; }
void UIElement::set_size(int w, int h) { if (w > 0) width_ = w; if (h > 0) height_ = h; }

Button::Button(int id, const string& name, int w, int h, const Style& s, const string& text)
    : UIElement(id, name, w, h, s), text_(text) {
}
string Button::type() const { return "Button"; }
string Button::extra() const { return text_; }
void Button::set_extra(const string& v) { if (!v.empty()) text_ = v; }

TextField::TextField(int id, const string& name, int w, int h, const Style& s, const string& ph)
    : UIElement(id, name, w, h, s), placeholder_(ph) {
}
string TextField::type() const { return "TextField"; }
string TextField::extra() const { return placeholder_; }
void TextField::set_extra(const string& v) { if (!v.empty()) placeholder_ = v; }

Label::Label(int id, const string& name, int w, int h, const Style& s, const string& text)
    : UIElement(id, name, w, h, s), text_(text) {
}
string Label::type() const { return "Label"; }
string Label::extra() const { return text_; }
void Label::set_extra(const string& v) { if (!v.empty()) text_ = v; }

Window::Window(const string& name, const string& title) : name_(name), title_(title) {}

void Window::edit(const string& name, const string& title) {
    if (!name.empty()) name_ = name;
    if (!title.empty()) title_ = title;
}
bool Window::contains(int id) const {
    return any_of(elements_.begin(), elements_.end(),
        [id](const shared_ptr<UIElement>& e) { return e->id() == id; });
}
bool Window::add(const shared_ptr<UIElement>& e) {
    if (!e || contains(e->id())) return false;
    elements_.push_back(e);
    return true;
}
bool Window::remove(int id) {
    auto it = find_if(elements_.begin(), elements_.end(),
        [id](const shared_ptr<UIElement>& e) { return e->id() == id; });
    if (it == elements_.end()) return false;
    elements_.erase(it);
    return true;
}

shared_ptr<UIElement> UIManager::find_by_id(int id) const {
    for (const auto& e : elements_) if (e->id() == id) return e;
    return nullptr;
}
Window* UIManager::find_window(const string& name) {
    for (auto& w : windows_) if (w.name() == name) return &w;
    return nullptr;
}
Style UIManager::style_by_name(const string& name) const {
    for (const auto& s : styles_) if (!s.empty() && s.name() == name) return s;
    return Style("Default");
}

UIManager::UIManager() {
    add_style("Default", "black", 12);
    add_style("Header", "blue", 24);
    add_element(1, "OkButton", 100, 30, "OK", "Default");
    add_element(2, "LoginField", 200, 25, "Введите логин", "Default");
    add_element(3, "Title", 300, 40, "Добро пожаловать", "Header");
    add_window("MainWindow", "Главное окно");
    attach("MainWindow", 1);
    attach("MainWindow", 2);
}

bool UIManager::add_element(int type, const string& name, int w, int h,
    const string& extra, const string& styleName) {
    if (name.empty() || w <= 0 || h <= 0) return false;
    Style s = style_by_name(styleName);
    shared_ptr<UIElement> e;
    if (type == 1) e = make_shared<Button>(nextId_, name, w, h, s, extra);
    else if (type == 2) e = make_shared<TextField>(nextId_, name, w, h, s, extra);
    else if (type == 3) e = make_shared<Label>(nextId_, name, w, h, s, extra);
    else return false;
    ++nextId_;
    elements_.push_back(e);
    return true;
}

bool UIManager::remove_element(int id) {
    auto it = find_if(elements_.begin(), elements_.end(),
        [id](const shared_ptr<UIElement>& e) { return e->id() == id; });
    if (it == elements_.end()) return false;
    for (auto& w : windows_) w.remove(id);
    elements_.erase(it);
    return true;
}

bool UIManager::edit_element(int id, const string& name, int w, int h,
    const string& extra, const string& styleName) {
    auto e = find_by_id(id);
    if (!e) return false;
    e->set_name(name);
    e->set_size(w, h);
    e->set_extra(extra);
    if (!styleName.empty()) e->set_style(style_by_name(styleName));
    return true;
}

vector<shared_ptr<UIElement>> UIManager::find_elements(const string& part) const {
    vector<shared_ptr<UIElement>> r;
    for (const auto& e : elements_) if (has(e->name(), part)) r.push_back(e);
    return r;
}

vector<shared_ptr<UIElement>> UIManager::filter_by_type(int type) const {
    vector<shared_ptr<UIElement>> r;
    for (const auto& e : elements_) if (e->type() == type_name(type)) r.push_back(e);
    return r;
}

vector<shared_ptr<UIElement>> UIManager::filter_by_window(const string& window) const {
    for (const auto& w : windows_) if (w.name() == window) return w.elements();
    return {};
}

void UIManager::sort_elements(int key) {
    sort(elements_.begin(), elements_.end(),
        [key](const shared_ptr<UIElement>& a, const shared_ptr<UIElement>& b) {
            switch (key) {
            case 2: return a->name() < b->name();
            case 3: return a->type() < b->type();
            case 4: return a->area() < b->area();
            default: return a->id() < b->id();
            }
        });
}

void UIManager::print_element_table(ostream& os, const vector<shared_ptr<UIElement>>& v) {
    os << pad("ID", 5) << pad("Тип", 11) << pad("Название", 16) << pad("Размер", 11)
        << pad("Стиль", 24) << "Текст/подсказка\n" << string(85, '-') << '\n';
    for (const auto& e : v) {
        const Style& s = e->style();
        os << pad(to_string(e->id()), 5) << pad(e->type(), 11) << pad(e->name(), 16)
            << pad(to_string(e->width()) + "x" + to_string(e->height()), 11)
            << pad(s.name() + "," + s.color() + "," + to_string(s.font_size()), 24)
            << e->extra() << '\n';
    }
    if (v.empty()) os << "(пусто)\n";
}

bool UIManager::add_window(const string& name, const string& title) {
    if (name.empty() || find_window(name)) return false;
    windows_.emplace_back(name, title);
    return true;
}

bool UIManager::remove_window(const string& name) {
    auto it = find_if(windows_.begin(), windows_.end(),
        [&](const Window& w) { return w.name() == name; });
    if (it == windows_.end()) return false;
    windows_.erase(it);
    return true;
}

bool UIManager::edit_window(const string& name, const string& newName, const string& newTitle) {
    Window* w = find_window(name);
    if (!w) return false;
    if (!newName.empty() && newName != name && find_window(newName)) return false;
    w->edit(newName, newTitle);
    return true;
}

bool UIManager::attach(const string& window, int elementId) {
    Window* w = find_window(window);
    auto e = find_by_id(elementId);
    return w && e && w->add(e);
}

bool UIManager::detach(const string& window, int elementId) {
    Window* w = find_window(window);
    return w && w->remove(elementId);
}

void UIManager::sort_windows(int key) {
    windows_.sort([key](const Window& a, const Window& b) {
        return key == 2 ? a.elements().size() < b.elements().size() : a.name() < b.name();
        });
}

void UIManager::print_windows(ostream& os, const string& filter) const {
    os << pad("Имя", 16) << pad("Заголовок", 22) << pad("Кол-во", 8) << "ID элементов\n"
        << string(70, '-') << '\n';
    bool any = false;
    for (const auto& w : windows_) {
        if (!has(w.name(), filter)) continue;
        any = true;
        string ids;
        for (const auto& e : w.elements()) ids += to_string(e->id()) + " ";
        os << pad(w.name(), 16) << pad(w.title(), 22)
            << pad(to_string(w.elements().size()), 8) << ids << '\n';
    }
    if (!any) os << "(пусто)\n";
}

bool UIManager::add_style(const string& name, const string& color, int size) {
    if (name.empty() || size <= 0) return false;
    for (const auto& s : styles_) if (s.name() == name) return false;
    for (auto& s : styles_)
        if (s.empty()) { s = Style(name, color.empty() ? "black" : color, size); return true; }
    return false;
}

bool UIManager::remove_style(int index) {
    if (index < 0 || index >= (int)styles_.size() || styles_[index].empty()) return false;
    styles_[index] = Style();
    return true;
}

bool UIManager::edit_style(int index, const string& name, const string& color, int size) {
    if (index < 0 || index >= (int)styles_.size() || styles_[index].empty()) return false;
    styles_[index].set(name, color, size);
    return true;
}

void UIManager::sort_styles(int key) {
    sort(styles_.begin(), styles_.end(), [key](const Style& a, const Style& b) {
        if (a.empty() || b.empty()) return !a.empty() && b.empty();
        return key == 2 ? a.font_size() < b.font_size() : a.name() < b.name();
        });
}

void UIManager::print_styles(ostream& os, const string& filter) const {
    os << pad("Слот", 6) << pad("Название", 16) << pad("Цвет", 12) << "Шрифт\n"
        << string(45, '-') << '\n';
    for (size_t i = 0; i < styles_.size(); ++i) {
        const Style& s = styles_[i];
        if (!filter.empty() && (s.empty() || !has(s.name(), filter))) continue;
        if (s.empty()) os << pad(to_string(i), 6) << "<пусто>\n";
        else os << pad(to_string(i), 6) << pad(s.name(), 16) << pad(s.color(), 12) << s.font_size() << '\n';
    }
}

void UIManager::print(ostream& os, int text) const {
    if (text == 1 || text == 4) { os << "\n=== ЭЛЕМЕНТЫ ===\n"; print_element_table(os, elements_); }
    if (text == 2 || text == 4) { os << "\n=== ОКНА ===\n"; print_windows(os); }
    if (text == 3 || text == 4) { os << "\n=== СТИЛИ ===\n"; print_styles(os); }
}

bool UIManager::save_to_file(const string& file, int text) const {
    ofstream f(file);
    if (!f) return false;
    print(f, text);
    return true;
}