/* -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
#ifndef _GSM_UTIL_H_
#define _GSM_UTIL_H_

#include <glib.h>
#include <gtk/gtk.h>
#include <string>

using std::string;

GtkWidget*
procman_make_label_for_mmaps_or_ofiles(const char *format,
                                       const char *process_name,
                                       unsigned pid);

gboolean
load_symbols(const char *module, ...) G_GNUC_NULL_TERMINATED;

const char*
format_process_state(guint state);

void
procman_debug_real(const char *file, int line, const char *func,
                   const char *format, ...) G_GNUC_PRINTF(4, 5);

#define procman_debug(FMT, ...) procman_debug_real(__FILE__, __LINE__, __func__, FMT, ##__VA_ARGS__)

gchar * get_monospace_system_font_name (void);

inline string make_string(char *c_str)
{
    if (!c_str) {
        procman_debug("NULL string");
        return string();
    }

    string s(c_str);
    g_free(c_str);
    return s;
}


namespace procman
{
    char* format_duration_for_display(unsigned centiseconds);

    void size_cell_data_func(GtkTreeViewColumn *col, GtkCellRenderer *renderer,
                             GtkTreeModel *model, GtkTreeIter *iter,
                             gpointer user_data);

    void size_na_cell_data_func(GtkTreeViewColumn *col, GtkCellRenderer *renderer,
                                GtkTreeModel *model, GtkTreeIter *iter,
                                gpointer user_data);

    void size_si_cell_data_func(GtkTreeViewColumn *col, GtkCellRenderer *renderer,
                                GtkTreeModel *model, GtkTreeIter *iter,
                                gpointer user_data);


    void duration_cell_data_func(GtkTreeViewColumn *col, GtkCellRenderer *renderer,
                                 GtkTreeModel *model, GtkTreeIter *iter,
                                 gpointer user_data);

    void time_cell_data_func(GtkTreeViewColumn *col, GtkCellRenderer *renderer,
                             GtkTreeModel *model, GtkTreeIter *iter,
                             gpointer user_data);

    void status_cell_data_func(GtkTreeViewColumn *col, GtkCellRenderer *renderer,
                               GtkTreeModel *model, GtkTreeIter *iter,
                               gpointer user_data);
    void priority_cell_data_func(GtkTreeViewColumn *col, GtkCellRenderer *renderer,
                               GtkTreeModel *model, GtkTreeIter *iter,
                               gpointer user_data);
    gint priority_compare_func(GtkTreeModel* model, GtkTreeIter* first,
                            GtkTreeIter* second, gpointer user_data);
    gint number_compare_func(GtkTreeModel* model, GtkTreeIter* first,
                            GtkTreeIter* second, gpointer user_data);


    template<typename T>
    void poison(T &t, char c)
    {
        memset(&t, c, sizeof t);
    }



    //
    // Stuff to update a tree_store in a smart way
    //

    template<typename T>
    void tree_store_update(GtkTreeModel* model, GtkTreeIter* iter, int column, const T& new_value)
    {
        T current_value;

        gtk_tree_model_get(model, iter, column, &current_value, -1);

        if (current_value != new_value)
            gtk_tree_store_set(GTK_TREE_STORE(model), iter, column, new_value, -1);
    }

    // undefined
    // catch every thing about pointers
    // just to make sure i'm not doing anything wrong
    template<typename T>
    void tree_store_update(GtkTreeModel* model, GtkTreeIter* iter, int column, T* new_value);

    // specialized versions for strings
    template<>
    void tree_store_update<const char>(GtkTreeModel* model, GtkTreeIter* iter, int column, const char* new_value);

    template<>
    inline void tree_store_update<char>(GtkTreeModel* model, GtkTreeIter* iter, int column, char* new_value)
    {
        tree_store_update<const char>(model, iter, column, new_value);
    }

    gchar* format_size(guint64 size, guint64 max = 0, bool want_bits = false);

    gchar* get_nice_level (gint nice);

    gchar* get_nice_level_with_priority (gint nice);

    std::string format_rate(guint64 rate, guint64 max_rate = 0, bool want_bits = false);

    std::string format_network(guint64 rate, guint64 max_rate = 0);
    std::string format_network_rate(guint64 rate, guint64 max_rate = 0);

    class NonCopyable
    {
    protected:
        NonCopyable() {}  // = default
        ~NonCopyable() {} // = default
    private:
        NonCopyable(const NonCopyable&)            /* = delete */;
        NonCopyable& operator=(const NonCopyable&) /* = delete */;
    };

}

#endif /* _GSM_UTIL_H_ */
