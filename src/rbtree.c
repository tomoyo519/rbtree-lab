#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  //트리의 root, nil 정보를 담을 포인터 변수 p
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  //닐노드도 노드이므로, 메모리 할당 해주기
  node_t *nilNode = (node_t *)calloc(1, sizeof(node_t));
  
  p->nil = nilNode; //포인터 변수와 닐노드연결
  p->root = p->nil; // root, 닐노드를 이어준다.(아직 삽입된 노드가 없을떄의 초기 ver.)
  p->nil->color = RBTREE_BLACK;// 닐노드의 색상은 항상 블랙이다.
  return p; // 새로운 rbtree 정보를 가진 포인터 변수 p를 반환한다.
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
