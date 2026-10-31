#pragma once
#include <array>
#include <list>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

class Style {
    std::string name_, color_;
    int font_size_;
public:
    Style(const std::string& name = "", const std::string& color = "black", int font_size = 12);
    const std::string& name() const { return name_; }
    const std::string& color() const { return color_; }
    int font_size() const { return font_size_; }
    bool empty() const { return name_.empty(); }
    void set(const std::string& name, const std::string& color, int font_size);
};

class UIElement {
protected:
    int id_;
    std::string name_;
    int width_, height_;
    Style style_;
public:
    UIElement(int id, const std::string& name, int w, int h, const Style& s);
    virtual ~UIElement() = default;

    virtual std::string type() const = 0;
    virtual std::string extra() const = 0;
    virtual void set_extra(const std::string& v) = 0;

    int id() const { return id_; }
    const std::string& name() const { return name_; }
    int width() const { return width_; }
    int height() const { return height_; }
    int area() const { return width_ * height_; }
    const Style& style() const { return style_; }

    void set_name(const std::string& n);
    void set_size(int w, int h);
    void set_style(const Style& s) { style_ = s; }
};

class Button : public UIElement {
    std::string text_;
public:
    Button(int id, const std::string& name, int w, int h, const Style& s, const std::string& text);
    std::string type() const override;
    std::string extra() const override;
    void set_extra(const std::string& v) override;
};

class TextField : public UIElement {
    std::string placeholder_;
public:
    TextField(int id, const std::string& name, int w, int h, const Style& s, const std::string& ph);
    std::string type() const override;
    std::string extra() const override;
    void set_extra(const std::string& v) override;
};

class Label : public UIElement {
    std::string text_;
public:
    Label(int id, const std::string& name, int w, int h, const Style& s, const std::string& text);
    std::string type() const override;
    std::string extra() const override;
    void set_extra(const std::string& v) override;
};

class Window {
    std::string name_, title_;
    std::vector<std::shared_ptr<UIElement>> elements_;
public:
    Window(const std::string& name, const std::string& title);
    const std::string& name() const { return name_; }
    const std::string& title() const { return title_; }
    const std::vector<std::shared_ptr<UIElement>>& elements() const { return elements_; }

    void edit(const std::string& name, const std::string& title);
    bool add(const std::shared_ptr<UIElement>& e);
    bool remove(int id);
    bool contains(int id) const;
};

class UIManager {
    std::vector<std::shared_ptr<UIElement>> elements_;
    std::list<Window> windows_;
    std::array<Style, 5> styles_;
    int nextId_ = 1;

    std::shared_ptr<UIElement> find_by_id(int id) const;
    Window* find_window(const std::string& name);
    Style style_by_name(const std::string& name) const;
public:
    UIManager();

    bool add_element(int type, const std::string& name, int w, int h,
        const std::string& extra, const std::string& styleName);
    bool remove_element(int id);
    bool edit_element(int id, const std::string& name, int w, int h,
        const std::string& extra, const std::string& styleName);
    std::vector<std::shared_ptr<UIElement>> find_elements(const std::string& part) const;
    std::vector<std::shared_ptr<UIElement>> filter_by_type(int type) const;
    std::vector<std::shared_ptr<UIElement>> filter_by_window(const std::string& window) const;
    void sort_elements(int key);
    static void print_element_table(std::ostream& os,
        const std::vector<std::shared_ptr<UIElement>>& v);

    bool add_window(const std::string& name, const std::string& title);
    bool remove_window(const std::string& name);
    bool edit_window(const std::string& name, const std::string& newName, const std::string& newTitle);
    bool attach(const std::string& window, int elementId);
    bool detach(const std::string& window, int elementId);
    void sort_windows(int key);
    void print_windows(std::ostream& os, const std::string& filter = "") const;

    bool add_style(const std::string& name, const std::string& color, int size);
    bool remove_style(int index);
    bool edit_style(int index, const std::string& name, const std::string& color, int size);
    void sort_styles(int key);
    void print_styles(std::ostream& os, const std::string& filter = "") const;

    void print(std::ostream& os, int option) const;
    bool save_to_file(const std::string& file, int option) const;
};
