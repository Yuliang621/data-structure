#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

// Binary tree node structure
struct BTreeNode {
    int value;
    struct BTreeNode* left;
    struct BTreeNode* right;
};

// Create a new node
struct BTreeNode* create_node(int value) {
    struct BTreeNode* node = (struct BTreeNode*)malloc(sizeof(struct BTreeNode));
    if (node == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Recursive function to build a binary tree
struct BTreeNode* build_binary_tree() {
    int value;
    printf("Enter node value (-1 for NULL): ");
    scanf("%d", &value);
    if (value == -1) {
        return NULL;
    }
    struct BTreeNode* root = create_node(value);
    printf("Enter left child of %d:\n", value);
    root->left = build_binary_tree();
    printf("Enter right child of %d:\n", value);
    root->right = build_binary_tree();
    return root;
}

// Recursive function to load binary tree data from a file
struct BTreeNode* load_tree_helper(FILE* file) {
    int value;
    if (fscanf(file, "%d", &value) != 1) {
        return NULL;
    }
    if (value == -1) {
        return NULL;
    }
    struct BTreeNode* root = create_node(value);
    root->left = load_tree_helper(file);
    root->right = load_tree_helper(file);
    return root;
}

// Load binary tree data from a file
struct BTreeNode* load_binary_tree_from_file(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Error: Unable to open file '%s' for reading.\n", file_path);
        return NULL;
    }
    // Recursive way to read binary tree data
    struct BTreeNode* root = load_tree_helper(file);
    fclose(file);
    return root;
}

// Find node with given value in the binary tree
struct BTreeNode* find_node(struct BTreeNode* root, int value) {
    if (root == NULL) {
        return NULL;
    }
    if (root->value == value) {
        return root;
    }
    // Recursively search left and right subtrees
    struct BTreeNode* left_result = find_node(root->left, value);
    struct BTreeNode* right_result = find_node(root->right, value);
    // Return the result from the left or right subtree
    return (left_result != NULL) ? left_result : right_result;
}

// Find lowest common ancestor of two nodes in the binary tree
struct BTreeNode* find_lca(struct BTreeNode* root, int node1, int node2) {
    if (root == NULL) {
        return NULL;
    }
    if (root->value == node1 || root->value == node2) {
        return root;
    }
    // Recursively search left and right subtrees
    struct BTreeNode* left_lca = find_lca(root->left, node1, node2);
    struct BTreeNode* right_lca = find_lca(root->right, node1, node2);
    // If both nodes are found in left and right subtrees, root is the LCA
    if (left_lca && right_lca) {
        return root;
    }
    // Otherwise, return the non-null result from left or right subtree
    return (left_lca != NULL) ? left_lca : right_lca;
}

// Find distance between node and target in the binary tree
int find_distance(struct BTreeNode* root, int target, int distance) {
    if (root == NULL) {
        return -1; // Target not found in this subtree
    }
    if (root->value == target) {
        return distance; // Target found at current node
    }
    // Recursively search left and right subtrees
    int left_distance = find_distance(root->left, target, distance + 1);
    int right_distance = find_distance(root->right, target, distance + 1);
    // Return the minimum distance found in left or right subtree
    return (left_distance != -1) ? left_distance : right_distance;
}

// Calculate the distance between two nodes in the binary tree
int get_length_between_nodes(struct BTreeNode* root, int node1, int node2) {
    if (root == NULL) {
        return 0;
    }

    // Find LCA of node1 and node2
    struct BTreeNode* lca = find_lca(root, node1, node2);

    // Get distance from LCA to node1
    int dist1 = find_distance(lca, node1, 0);
    // Get distance from LCA to node2
    int dist2 = find_distance(lca, node2, 0);

    // Total distance between node1 and node2
    return dist1 + dist2;
}

// Save the binary tree to a file
void save_binary_tree_to_file(const char* file_path, struct BTreeNode* root) {
    FILE* outFile = fopen(file_path, "w");
    if (outFile == NULL) {
        printf("Error: Unable to open file '%s' for writing.\n", file_path);
        return;
    }
    // Save binary tree data to the file using pre-order traversal
    struct stack {
        struct BTreeNode* data;
        struct stack* next;
    };
    struct stack* st = NULL;
    struct stack* temp;

    fprintf(outFile, "%d ", root->value);
    if (root->left != NULL) {
        fprintf(outFile, "%d ", root->left->value);
        temp = (struct stack*)malloc(sizeof(struct stack));
        temp->data = root->left;
        temp->next = st;
        st = temp;
    }
    else {
        fprintf(outFile, "-1 ");
    }
    if (root->right != NULL) {
        fprintf(outFile, "%d ", root->right->value);
        temp = (struct stack*)malloc(sizeof(struct stack));
        temp->data = root->right;
        temp->next = st;
        st = temp;
    }
    else {
        fprintf(outFile, "-1 ");
    }
    while (st != NULL) {
        temp = st;
        st = st->next;
        fprintf(outFile, "%d ", temp->data->value);
        if (temp->data->left != NULL) {
            fprintf(outFile, "%d ", temp->data->left->value);
            struct stack* new_temp = (struct stack*)malloc(sizeof(struct stack));
            new_temp->data = temp->data->left;
            new_temp->next = st;
            st = new_temp;
        }
        else {
            fprintf(outFile, "-1 ");
        }
        if (temp->data->right != NULL) {
            fprintf(outFile, "%d ", temp->data->right->value);
            struct stack* new_temp = (struct stack*)malloc(sizeof(struct stack));
            new_temp->data = temp->data->right;
            new_temp->next = st;
            st = new_temp;
        }
        else {
            fprintf(outFile, "-1 ");
        }
        free(temp);
    }
    fclose(outFile);
}
// Print the binary tree sequence (preorder traversal)
void print_binary_tree_preorder(struct BTreeNode* root) {
    if (root == NULL) {
        return;
    }
    printf("%d ", root->value);
    print_binary_tree_preorder(root->left);
    print_binary_tree_preorder(root->right);
}


// Print the binary tree sequence (inorder traversal)
void print_binary_tree(struct BTreeNode* root) {
    if (root == NULL) {
        return;
    }

    // 使用中序遍历
    print_binary_tree(root->left);

    printf("%d ", root->value);
    print_binary_tree(root->right);
}
// Print the binary tree sequence (postorder traversal)
void print_binary_tree_postorder(struct BTreeNode* root) {
    if (root == NULL) {
        return;
    }
    print_binary_tree_postorder(root->left);
    print_binary_tree_postorder(root->right);
    printf("%d ", root->value);
}
// Find parent node of a given node in the binary tree
struct BTreeNode* find_parent_node(struct BTreeNode* root, int value) {
    if (root == NULL || (root->left != NULL && root->left->value == value) || (root->right != NULL && root->right->value == value)) {
        return root;
    }
    struct BTreeNode* left_parent = find_parent_node(root->left, value);
    if (left_parent != NULL) {
        return left_parent;
    }
    return find_parent_node(root->right, value);
}

// Find children nodes of a given node in the binary tree
void find_children_nodes(struct BTreeNode* root, int value) {
    if (root == NULL) {
        return;
    }
    if (root->value == value) {
        printf("Children nodes of %d: ", value);
        if (root->left != NULL) {
            printf("%d ", root->left->value);
        }
        if (root->right != NULL) {
            printf("%d ", root->right->value);
        }
        printf("\n");
        return;
    }
    find_children_nodes(root->left, value);
    find_children_nodes(root->right, value);
}

// Print the binary tree shape with parent-child connections
void print_tree_shape(struct BTreeNode* root, int level) {
    if (root == NULL) {
        return;
    }
    // 递归打印右子树，缩进增加
    print_tree_shape(root->right, level + 1);
    // 打印当前节点和其父子节点连接关系
    for (int i = 0; i < level; i++) {
        printf("    ");
    }
    if (level > 0) {
        printf("|--");
    }
    printf("%d\n", root->value);
    // 递归打印左子树，缩进增加
    print_tree_shape(root->left, level + 1);
}

// Calculate the distance between two nodes in the binary tree (Recursive)
int calculate_distance_recursive(struct BTreeNode* root, int node1, int node2) {
    if (root == NULL) {
        return -1; // Node not found in this subtree
    }
    // Find LCA (Lowest Common Ancestor) of node1 and node2
    struct BTreeNode* lca = find_lca(root, node1, node2);

    if (lca == NULL) {
        return -1; // One or both nodes not found
    }

    // Get distance of node1 from lca and node2 from lca
    int dist1 = find_distance(lca, node1, 0);
    int dist2 = find_distance(lca, node2, 0);

    if (dist1 == -1 || dist2 == -1) {
        return -1; // One or both nodes not found
    }

    return dist1 + dist2; // Distance is sum of distances from lca
}

// Display the main menu
void show_menu() {
    printf("\nBinary Tree Distance Calculator\n");
    printf("1. Manual input of binary tree and save to file\n");
    printf("2. Load binary tree from file\n");
    printf("3. Calculate distance between nodes (Recursive)\n");
    printf("4. Calculate distance between nodes (Non-recursive)\n");
    printf("5. Print current binary tree sequence\n");
    printf("6. Display saved binary tree shape\n");
    printf("7. Enter node value to find its parent and children nodes\n");
    printf("8. Exit program\n");
    printf("Enter your choice: ");
}

int main() {
    int choice;
    struct BTreeNode* root = NULL;
    const char* file_path = "binary_tree.txt";

    do {
        show_menu();
        scanf("%d", &choice);
        switch (choice) {
        case 1: {
            printf("Enter the binary tree:\n");
            root = build_binary_tree();

            // 更新并保存二叉树到文件
            printf("Saving binary tree to file '%s'...\n", file_path);
            save_binary_tree_to_file(file_path, root);
            printf("Binary tree saved successfully.\n");

            break;
        }

        case 2: {
            root = load_binary_tree_from_file(file_path);
            break;
        }
        case 3: {
            if (root != NULL) {
                int node1, node2;
                printf("Enter first node: ");
                scanf("%d", &node1);
                printf("Enter second node: ");
                scanf("%d", &node2);
                int distance = calculate_distance_recursive(root, node1, node2);
                if (distance != -1) {
                    printf("Distance between node %d and node %d is %d\n", node1, node2, distance);
                }
                else {
                    printf("One or both of the nodes do not exist in the tree.\n");
                }
            }
            else {
                printf("Error: Binary tree not loaded. Please load a binary tree first.\n");
            }
            break;
        }
        case 4: {
            // Implement non-recursive distance calculation here
            printf("Non-recursive distance calculation is not implemented yet.\n");
            break;
        }
        case 5: {
            if (root != NULL) {
                int traversal_choice;
                printf("Choose traversal type:\n");
                printf("1. Inorder\n");
                printf("2. Preorder\n");
                printf("3. Postorder\n");
                printf("Enter your choice: ");
                scanf("%d", &traversal_choice);

                switch (traversal_choice) {
                case 1:
                    printf("Binary tree sequence (inorder): ");
                    print_binary_tree(root);
                    printf("\n");
                    break;
                case 2:
                    printf("Binary tree sequence (preorder): ");
                    print_binary_tree_preorder(root);
                    printf("\n");
                    break;
                case 3:
                    printf("Binary tree sequence (postorder): ");
                    print_binary_tree_postorder(root);
                    printf("\n");
                    break;
                default:
                    printf("Invalid choice.\n");
                }
            }
            else {
                printf("Error: Binary tree not loaded. Please load a binary tree first.\n");
            }
            break;
        }
        case 6: {
            if (root != NULL) {
                printf("Binary tree shape:\n");
                print_tree_shape(root, 0);
            }
            else {
                printf("Error: Binary tree not loaded. Please load a binary tree first.\n");
            }
            break;
        }
        case 7: {
            if (root != NULL) {
                int node_value;
                printf("Enter node value to find its parent and children nodes: ");
                scanf("%d", &node_value);

                // Find parent node
                struct BTreeNode* parent_node = find_parent_node(root, node_value);
                if (parent_node != NULL) {
                    printf("Parent node of %d: ", node_value);
                    if (parent_node->value == node_value) {
                        printf("None (Root node)\n");
                    }
                    else {
                        printf("%d\n", parent_node->value);
                    }
                }
                else {
                    printf("Node %d not found in the binary tree.\n", node_value);
                }

                // Find children nodes
                find_children_nodes(root, node_value);
            }
            else {
                printf("Error: Binary tree not loaded. Please load a binary tree first.\n");
            }
            break;
        }
        case 8:
            printf("Exiting program.\n");
            break;
        default:
            printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 8);

    return 0;
}

