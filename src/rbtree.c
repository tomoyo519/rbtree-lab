#include "rbtree.h"
#include <stdio.h>
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
  // 트리를 순회하면서 각 노드의 메모리를 반환하는 함수
  free(t);
  node_t *cur = t->root;
  while(cur != t->nil){
    
  }
}

void right_rotate(rbtree *t, node_t *z){
  printf("right_rotate");
}

void left_rotate(rbtree *t, node_t *z){
  printf("left_rotate");
}

void exchange_color(node_t *x, node_t *y){
  printf("exchange_color");
}

void  rbtree_insert_fixup(rbtree*t, node_t *z){
  //insert 후 rbtree 규칙에 어긋난 것이 있다면 정렬하는 함수
  //red의 자식은 black만 가능하다.
  //모든 경로에서 발생하는 블랙의 숫자는 동일하다.
  //루트의 색상은 검정이다.
  // 불균형인 경우 3가지
  // 1) 삼촌노드가 red 인 경우 : (부모 레드 && 삼촌 레드)
  // -> 부모와 삼촌 노드를 블랙으로 변경하기
  // ->할아버지를 레드로 바꾼뒤, 할아버지에서 부터 다시 확인 시작(할아버지 노드를 추가된 노드로 설정하고 불균형 복구 함수를 재귀적으로 호출한다.)

  // 2) 삽입된 노드가 부모의 오른쪽(왼쪽)자녀이고, 부모도 레드, 할아버지의 왼쪽(오른쪽) 자녀이고 삼촌은 블랙이라면
  // ->부모를 기준으로 왼쪽(오른쪽) 회전한 뒤, case 3으로 해결

  // 3) 삽입된 레드 노드가 부모의 왼쪽(오른쪽) 자녀, 부모도 레드이고 할아버지의 왼쪽(오른쪽)자녀
  // 삼촌은 블랙이라면,  부모와 할아버지의 색을 바꾼 후 , 할아버지 기준으로 오른쪽으로 회전.


node_t *parent = z->parent;
node_t *grand_parent = parent->parent;
node_t *uncle;

int is_left = (z == parent ->left); // 추가한 노드가 왼쪽 자식인지 여부
int is_parent_is_left; // 부모가 왼쪽 자식인지 여부

//z가 루트노드일 경우, 색상만 변경해준다.
// ??? z printf 찍어보면 뭐가 나오나 ? 주소값.
if(z == t->root){// 같은 것을 가리킵니까?
  z->color = RBTREE_BLACK;
  return;
}

//부모가 black 인 경우  = 변경없음
if(parent->color == RBTREE_BLACK){
  return;
}
is_parent_is_left = (grand_parent->left == parent);

// 부모가 왼쪽자식이면 삼촌은 조부모의 오른쪽 자식이다.
uncle = (is_parent_is_left) ? grand_parent->right : grand_parent->left;

// case 1 : 부모와 부모의 형제가 모두 red 인 경우, 나도 레드.
if( uncle->color == RBTREE_RED ) {
  parent->color = RBTREE_BLACK;
  uncle->color = RBTREE_BLACK;
  grand_parent->color = RBTREE_RED;
  rbtree_insert_fixup(t, grand_parent);
  return;
}


// 위의 경우에서 삼촌 컬러가 레드인경우를 줬으므로 아래경우는 무조건 삼촌 컬러가 블랙인 경우임 
if(is_parent_is_left){
  if(is_left){
  // case 2 : 부모의 형제가 블랙 && 부모가 왼쪽 자식 && 노드가 왼쪽 자식인 경우, 
    right_rotate(t, parent);
    exchange_color(parent, parent->right);
  }else{
    //case 3: 삼촌노드가 블랙 && 부모노드가 왼쪽 && 새노드가 오른쪽 자식인경우,
    left_rotate(t, z); // ??? z 가 변경됨.
    // ??? 왜 왼쪽 회전 했다가 오른쪽 회전함.. -> 왼쪽 회전을 하면 case2가 됨.
    right_rotate(t, z);
    // 새노드와 자식노드의 색상을 바꾼다. 
    //z가 무엇인가. 새로 삽입할 노드임. 근데 left_right를 하면 z가 변경되는가 ??? 
    exchange_color(z, z->right);
    return;
  }

  if(is_left){
    //case 3 : 삼촌이 블랙이고, 내가 왼쪽의 자식이고,, 부모노드가 오른쪽에 있는경우,
    right_rotate(t,z);
    left_rotate(t, z);
    exchange_color(z, z->left);
    return;
  }
  //case 2 : 부모의 형재가 블랙이고 내 부모가 오른쪽 노드이고, 내가 오른쪽 노드임.
  left_rotate(t,parent);
  //부모노드와 형제노드의 색상을 바꾼다.
  exchange_color(parent, parent->left);
}




}

//인자로 받는 rbtree *t 는 현재 만들어져있는 rbtree를 가리키는 포인터이다. ???
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z->key = key;
  //자리를 찾아 내려갈 ptr 변수 x,y
  node_t *x = t->root;
  node_t *y = t->nil;
  //이때까지 while문을 돌려야 y는 nil 직전의 노드를 가리킬 수 있다.
  while (x != t->nil){
    //while 문이 매번 새로 돌 떄마다, x가 가리키는 값을 y가 가리키게 바꿔준다.
    //그래야 x가 nil을 만났을때 y는 nil 직전의 노드를 가리킬 수 있다.
    y=x;
    if(x->key > key){
      //왼쪽 탐색
      x = x->left;
    }
    else{
      x = x->right;
    }
  }
  // y는 닐 노드의 직전의 노드를 가리킨다. = 자리를 찾았다! 부모 자식 관계를 연결해준다.
  z->parent = y;
  //만약 들어갈 자리가 닐노드라면, 최초의 노드이다.

  if(y == t->nil){
    t->root = z;
  }

  //닐노드가 아닌경우 key값에 따라 왼쪽값 오른쪽값을 정해준다.
  if(y->key < key){
    y->right = z;
    //y->right = z>key ??? 가 아닌가 ->왜: y->right는 구조체 포인터이다. 값이 아니라 주소를 가르켜야 한다.
  }else{
    y->left = z;
  }
  // 닐노드 명시해주기
  z->left = t->nil;
  z->right = t->nil;

  // 
  z->color = RBTREE_RED;
  rbtree_insert_fixup(t, z);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  // 탐색의 시간복잡도 = logN
  node_t *cur = t->root;
  while( cur != t->nil){
    if(cur->key == key){
      return cur;
    }else{
      cur = (key < cur->key) ? cur->left : cur->right;
    }
  }
// 찾는 경우가 없는 경우 null 반환
  return NULL;
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
