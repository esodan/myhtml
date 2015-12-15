//
//  myhtml_tree.h
//  myhtml
//
//  Created by Alexander Borisov on 08.10.15.
//  Copyright © 2015 Alexander Borisov. All rights reserved.
//

#ifndef myhtml_tree_h
#define myhtml_tree_h

#include "myosi.h"
#include "myhtml.h"
#include "myhtml_token.h"
#include "myhtml_string.h"
#include "mcobject.h"
#include "mcsync.h"
#include "mcobject_async.h"

#define myhtml_tree_get(__tree__, __attr__) __tree__->__attr__
#define myhtml_tree_set(__tree__, __attr__) __tree__->__attr__

#define myhtml_tree_token_current(__tree__) myhtml_tree_get(__tree__, token_current)
#define myhtml_tree_token_attr_current(__tree__) myhtml_tree_get(__tree__, attr_current)

#define myhtml_tree_node_get(__tree__, __node_id__, __attr__) __tree__->nodes[__node_id__].__attr__

enum myhtml_tree_node_type {
    MyHTML_TYPE_NONE    = 0,
    MyHTML_TYPE_BLOCK   = 1,
    MyHTML_TYPE_INLINE  = 2,
    MyHTML_TYPE_TABLE   = 3,
    MyHTML_TYPE_META    = 4,
    MyHTML_TYPE_COMMENT = 5
};

enum myhtml_close_type {
    MyHTML_CLOSE_TYPE_NONE  = 0,
    MyHTML_CLOSE_TYPE_NOW   = 1,
    MyHTML_CLOSE_TYPE_SELF  = 2,
    MyHTML_CLOSE_TYPE_BLOCK = 3
};

enum myhtml_tree_node_flags {
    MyHTML_TREE_NODE_UNDEF           = 0,
    MyHTML_TREE_NODE_PARSER_INSERTED = 1,
    MyHTML_TREE_NODE_BLOCKING        = 2
};

struct myhtml_tree_node {
    enum myhtml_tree_node_flags flags;
    
    mytags_ctx_index_t tag_idx;
    enum myhtml_namespace namespace;
    
    myhtml_tree_node_t* prev;                // предыдущий ид элемента этого же уровня
    myhtml_tree_node_t* next;                // следеющий ид эелента этого же уровня
    myhtml_tree_node_t* child;               // ид чилда
    myhtml_tree_node_t* parent;              // ид родителя
    
    myhtml_tree_node_t* last_child;          //
    
    myhtml_token_node_t* token;
};

enum myhtml_tree_compat_mode {
    MyHTML_TREE_COMPAT_MODE_NO_QUIRKS       = 0x00,
    MyHTML_TREE_COMPAT_MODE_QUIRKS          = 0x01,
    MyHTML_TREE_COMPAT_MODE_LIMITED_QUIRKS  = 0x02
};

enum myhtml_tree_doctype_id {
    MyHTML_TREE_DOCTYPE_ID_NAME   = 0x00,
    MyHTML_TREE_DOCTYPE_ID_SYSTEM = 0x01,
    MyHTML_TREE_DOCTYPE_ID_PUBLIC = 0x02
};

enum myhtml_tree_flags {
    MyHTML_TREE_FLAGS_CLEAN           = 0x00,
    MyHTML_TREE_FLAGS_SCRIPT          = 0x01,
    MyHTML_TREE_FLAGS_FRAMESET_OK     = 0x02,
    MyHTML_TREE_FLAGS_IFRAME_SRCDOC   = 0x04,
    MyHTML_TREE_FLAGS_FRAGMENT        = 0x08,
    MyHTML_TREE_FLAGS_ALREADY_STARTED = 0x10
};

enum myhtml_tree_insertion_mode {
    MyHTML_TREE_INSERTION_MODE_DEFAULT     = 0x00,
    MyHTML_TREE_INSERTION_MODE_BEFORE      = 0x01,
    MyHTML_TREE_INSERTION_MODE_AFTER       = 0x02
};

struct myhtml_tree_indexes {
    mytags_index_t* tags;
};

struct myhtml_tree_list {
    myhtml_tree_node_t** list;
    size_t length;
    size_t size;
};

struct myhtml_tree_token_list {
    myhtml_token_node_t** list;
    size_t length;
    size_t size;
};

struct myhtml_tree_insertion_list {
    enum myhtml_insertion_mode* list;
    size_t length;
    size_t size;
};

struct myhtml_tree {
    myhtml_token_t* token;
    myhtml_token_attr_t* attr_current;
    
    mcobject_async_t* nodes_obj; // myhtml_tree_node_t
    
    myhtml_tree_indexes_t* indexes;
    
    myhtml_tree_node_t* current;
    myhtml_tree_node_t* document;
    myhtml_tree_node_t* fragment;
    
    myhtml_tree_node_t* node_head;
    myhtml_tree_node_t* node_form;
    
    myhtml_queue_node_index_t queue;
    
    myhtml_tree_list_t* active_formatting;
    myhtml_tree_list_t* open_elements;
    myhtml_tree_list_t* other_elements;
    myhtml_tree_token_list_t* token_list;
    myhtml_tree_insertion_list_t* template_insertion;
    
    myhtml_token_attr_t* queue_attr;
    
    enum myhtml_tokenizer_state state;
    enum myhtml_insertion_mode insert_mode;
    enum myhtml_insertion_mode orig_insert_mode;
    enum myhtml_tree_compat_mode compat_mode;
    enum myhtml_tree_flags flags;
    mybool_t foster_parenting;
    
    myhtml_t* myhtml;
    mytags_ctx_index_t tmp_tag_id;
};

// base
myhtml_tree_t * myhtml_tree_init(myhtml_t* myhtml);
void myhtml_tree_clean(myhtml_tree_t* tree);
myhtml_tree_t * myhtml_tree_destroy(myhtml_tree_t* tree);

// list
myhtml_tree_list_t * myhtml_tree_list_init(void);
void myhtml_tree_list_clean(myhtml_tree_list_t* list);
myhtml_tree_list_t * myhtml_tree_list_destroy(myhtml_tree_list_t* list, mybool_t destroy_self);

void myhtml_tree_list_append(myhtml_tree_list_t* list, myhtml_tree_node_t* node);
void myhtml_tree_list_append_after_index(myhtml_tree_list_t* list, myhtml_tree_node_t* node, size_t index);
myhtml_tree_node_t * myhtml_tree_list_current_node(myhtml_tree_list_t* list);

// token list
myhtml_tree_token_list_t * myhtml_tree_token_list_init(void);
void myhtml_tree_token_list_clean(myhtml_tree_token_list_t* list);
myhtml_tree_token_list_t * myhtml_tree_token_list_destroy(myhtml_tree_token_list_t* list, mybool_t destroy_self);

void myhtml_tree_token_list_append(myhtml_tree_token_list_t* list, myhtml_token_node_t* token);
void myhtml_tree_token_list_append_after_index(myhtml_tree_token_list_t* list, myhtml_token_node_t* token, size_t index);
myhtml_token_node_t * myhtml_tree_token_list_current_node(myhtml_tree_token_list_t* list);

// active formatting
myhtml_tree_list_t * myhtml_tree_active_formatting_init(myhtml_tree_t* tree);
void myhtml_tree_active_formatting_clean(myhtml_tree_t* tree);
myhtml_tree_list_t * myhtml_tree_active_formatting_destroy(myhtml_tree_t* tree);

mybool_t myhtml_tree_active_formatting_is_marker(myhtml_tree_t* tree, myhtml_tree_node_t* idx);
myhtml_tree_node_t* myhtml_tree_active_formatting_between_last_marker(myhtml_tree_t* tree, mytags_ctx_index_t tag_idx, size_t* return_idx);

void myhtml_tree_active_formatting_append(myhtml_tree_t* tree, myhtml_tree_node_t* node);
void myhtml_tree_active_formatting_append_with_check(myhtml_tree_t* tree, myhtml_tree_node_t* node);
void myhtml_tree_active_formatting_pop(myhtml_tree_t* tree);
void myhtml_tree_active_formatting_remove(myhtml_tree_t* tree, myhtml_tree_node_t* node);
void myhtml_tree_active_formatting_remove_by_index(myhtml_tree_t* tree, size_t idx);

void myhtml_tree_active_formatting_reconstruction(myhtml_tree_t* tree);
void myhtml_tree_active_formatting_up_to_last_marker(myhtml_tree_t* tree);

mybool_t myhtml_tree_active_formatting_find(myhtml_tree_t* tree, myhtml_tree_node_t* idx, size_t* return_idx);
myhtml_tree_node_t* myhtml_tree_active_formatting_current_node(myhtml_tree_t* tree);

// open elements
myhtml_tree_list_t * myhtml_tree_open_elements_init(myhtml_tree_t* tree);
void myhtml_tree_open_elements_clean(myhtml_tree_t* tree);
myhtml_tree_list_t * myhtml_tree_open_elements_destroy(myhtml_tree_t* tree);

myhtml_tree_node_t* myhtml_tree_current_node(myhtml_tree_t* tree);
myhtml_tree_node_t * myhtml_tree_adjusted_current_node(myhtml_tree_t* tree);

void myhtml_tree_open_elements_append(myhtml_tree_t* tree, myhtml_tree_node_t* node);
void myhtml_tree_open_elements_append_after_index(myhtml_tree_t* tree, myhtml_tree_node_t* node, size_t index);
void myhtml_tree_open_elements_pop(myhtml_tree_t* tree);
void myhtml_tree_open_elements_pop_until(myhtml_tree_t* tree, mytags_ctx_index_t tag_idx, mybool_t is_exclude);
void myhtml_tree_open_elements_pop_until_by_node(myhtml_tree_t* tree, myhtml_tree_node_t* node_idx, mybool_t is_exclude);
void myhtml_tree_open_elements_pop_until_by_index(myhtml_tree_t* tree, size_t idx, mybool_t is_exclude);
void myhtml_tree_open_elements_remove(myhtml_tree_t* tree, myhtml_tree_node_t* node);

mybool_t myhtml_tree_open_elements_find(myhtml_tree_t* tree, myhtml_tree_node_t* idx, size_t* pos);
myhtml_tree_node_t * myhtml_tree_open_elements_find_by_tag_idx(myhtml_tree_t* tree, mytags_ctx_index_t tag_idx, size_t* return_index);
myhtml_tree_node_t * myhtml_tree_element_in_scope(myhtml_tree_t* tree, mytags_ctx_index_t tag_idx, enum mytags_categories category);
mybool_t myhtml_tree_element_in_scope_by_node(myhtml_tree_t* tree, myhtml_tree_node_t* node, enum mytags_categories category);
void myhtml_tree_generate_implied_end_tags(myhtml_tree_t* tree, mytags_ctx_index_t exclude_tag_idx);
void myhtml_tree_generate_all_implied_end_tags(myhtml_tree_t* tree, mytags_ctx_index_t exclude_tag_idx);
myhtml_tree_node_t * myhtml_tree_appropriate_place_inserting(myhtml_tree_t* tree, myhtml_tree_node_t* override_target, enum myhtml_tree_insertion_mode* mode);

// template insertion
myhtml_tree_insertion_list_t * myhtml_tree_template_insertion_init(myhtml_tree_t* tree);
void myhtml_tree_template_insertion_clean(myhtml_tree_t* tree);
myhtml_tree_insertion_list_t * myhtml_tree_template_insertion_destroy(myhtml_tree_t* tree);

void myhtml_tree_template_insertion_append(myhtml_tree_t* tree, enum myhtml_insertion_mode insert_mode);
void myhtml_tree_template_insertion_pop(myhtml_tree_t* tree);

void myhtml_tree_reset_insertion_mode_appropriately(myhtml_tree_t* tree);

mybool_t myhtml_tree_adoption_agency_algorithm(myhtml_tree_t* tree, mytags_ctx_index_t subject_tag_idx);
size_t myhtml_tree_template_insertion_length(myhtml_tree_t* tree);

// other for a tree
myhtml_tree_node_t * myhtml_tree_node_create(myhtml_tree_t* tree);
void myhtml_tree_node_delete(myhtml_tree_t* tree, myhtml_tree_node_t* idx);
void myhtml_tree_node_clean(myhtml_tree_node_t* tree_node);
myhtml_tree_node_t * myhtml_tree_node_clone(myhtml_tree_t* tree, myhtml_tree_node_t* idx, size_t thread_idx);

void myhtml_tree_print_by_tree_idx(myhtml_tree_t* tree, myhtml_tree_node_t* idx, FILE* out, size_t inc);

void myhtml_tree_node_add_child(myhtml_tree_t* myhtml_tree, myhtml_tree_node_t* root, myhtml_tree_node_t* idx);
void myhtml_tree_node_insert_before(myhtml_tree_t* myhtml_tree, myhtml_tree_node_t* root, myhtml_tree_node_t* idx);
void myhtml_tree_node_insert_after(myhtml_tree_t* myhtml_tree, myhtml_tree_node_t* root, myhtml_tree_node_t* idx);
void myhtml_tree_node_insert_by_mode(myhtml_tree_t* tree, myhtml_tree_node_t* adjusted_location, myhtml_tree_node_t* node, enum myhtml_tree_insertion_mode mode);
void myhtml_tree_node_remove(myhtml_tree_t* tree, myhtml_tree_node_t* idx);

myhtml_tree_node_t * myhtml_tree_node_insert_html_element(myhtml_tree_t* tree, myhtml_token_node_t* token);
myhtml_tree_node_t * myhtml_tree_node_insert_foreign_element(myhtml_tree_t* tree, myhtml_token_node_t* token);
myhtml_tree_node_t * myhtml_tree_node_insert_by_token(myhtml_tree_t* tree, myhtml_token_node_t* token, enum myhtml_namespace my_namespace);
myhtml_tree_node_t * myhtml_tree_node_insert(myhtml_tree_t* tree, mytags_ctx_index_t tag_idx, enum myhtml_namespace my_namespace);
myhtml_tree_node_t * myhtml_tree_node_insert_by_node(myhtml_tree_t* tree, myhtml_tree_node_t* idx);
myhtml_tree_node_t * myhtml_tree_node_insert_comment(myhtml_tree_t* tree, myhtml_token_node_t* token, myhtml_tree_node_t* parent);
myhtml_tree_node_t * myhtml_tree_node_insert_doctype(myhtml_tree_t* tree, myhtml_token_node_t* token);
myhtml_tree_node_t * myhtml_tree_node_insert_root(myhtml_tree_t* tree, myhtml_token_node_t* token, enum myhtml_namespace my_namespace);
myhtml_tree_node_t * myhtml_tree_node_insert_text(myhtml_tree_t* tree, myhtml_token_node_t* token);

myhtml_tree_indexes_t * myhtml_tree_index_create(myhtml_tree_t* tree, mytags_t* mytags);

// other

void myhtml_tree_tags_close_p(myhtml_tree_t* tree);
myhtml_tree_node_t * myhtml_tree_generic_raw_text_element_parsing_algorithm(myhtml_tree_t* tree, myhtml_token_node_t* token_node);
void myhtml_tree_clear_stack_back_table_context(myhtml_tree_t* tree);
void myhtml_tree_clear_stack_back_table_body_context(myhtml_tree_t* tree);
void myhtml_tree_clear_stack_back_table_row_context(myhtml_tree_t* tree);
void myhtml_tree_close_cell(myhtml_tree_t* tree, myhtml_tree_node_t* tr_or_th_node);

mybool_t myhtml_tree_is_mathml_integration_point(myhtml_tree_t* tree, myhtml_tree_node_t* node);
mybool_t myhtml_tree_is_html_integration_point(myhtml_tree_t* tree, myhtml_tree_node_t* node);

#endif /* myhtml_tree_h */

