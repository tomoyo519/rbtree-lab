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


void traverse_and_delete_node(rbtree *t, node_t *node){
  if(node->left != t->nil){
    traverse_and_delete_node(t, node->left);
  }if (node->right != t->nil){
    traverse_and_delete_node(t, node->right);
  }
  //현재 노드의 메모리를 반환
  free(node);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  // 트리를 순회하면서 각 노드의 메모리를 반환하는 함수
  free(t);
  node_t *node = t->root;
  if (node != t->nil){
    traverse_and_delete_node(t, node);

  };
  // 닐노드와 rb트리 구조체의 메모리를 반환
  free(t->nil);
  free(t);
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


node_t *get_next_node(const rbtree *t, node_t *p)
{
  node_t *current = p->right;
  if (current == t->nil) // 오른쪽 자식이 없으면
  {
    current = p;
    while (1)
    {
      if (current->parent->right == current) // current가 오른쪽 자식인 경우
        current = current->parent;           // 부모 노드로 이동 후 이어서 탐색
      else
        return current->parent; // current가 왼쪽 자식인 경우 부모 리턴
    }
  }
  while (current->left != t->nil) // 왼쪽 자식이 있으면
    current = current->left;      // 왼쪽 끝으로 이동
  return current;
}

void rbtree_erase_fixup(rbtree *t, node_t *parent, int is_left)
{
  // 삭제 후 대체한 노드가 RED (Red & Black): BLACK으로 변경
  node_t *extra_black = is_left ? parent->left : parent->right;
  if (extra_black->color == RBTREE_RED)
  {
    extra_black->color = RBTREE_BLACK;
    return;
  }

  node_t *sibling = is_left ? parent->right : parent->left;
  node_t *sibling_left = sibling->left;
  node_t *sibling_right = sibling->right;

  if (sibling->color == RBTREE_RED)
  { // [CASE D3] 형제가 RED
    if (is_left)
      left_rotate(t, sibling);
    else
      right_rotate(t, sibling);
    exchange_color(sibling, parent);
    rbtree_erase_fixup(t, parent, is_left);
    return;
  }

  node_t *near = is_left ? sibling_left : sibling_right;    // 형제의 자식 중 extra_black으로부터 가까운 노드
  node_t *distant = is_left ? sibling_right : sibling_left; // 형제의 자식 중 extra_black으로부터 먼 노드

  if (is_left && near->color == RBTREE_RED && distant->color == RBTREE_BLACK)
  { // [CASE D4] 형제가 BLACK, 형제의 가까운 자식이 RED, 형제의 더 먼 자식이 BLACK
    right_rotate(t, near);
    exchange_color(sibling, near);
    rbtree_erase_fixup(t, parent, is_left);
    return;
  }

  if (is_left && distant->color == RBTREE_RED)
  { // [CASE D5] 형제가 BLACK, 형제의 더 먼 자식이 RED
    left_rotate(t, sibling);
    exchange_color(sibling, parent);
    distant->color = RBTREE_BLACK;
    return;
  }

  if (near->color == RBTREE_RED && distant->color == RBTREE_BLACK)
  { // [CASE D4] 형제가 BLACK, 형제의 가까운 자식이 RED, 형제의 더 먼 자식이 BLACK
    left_rotate(t, near);
    exchange_color(sibling, near);
    rbtree_erase_fixup(t, parent, is_left);
    return;
  }

  if (distant->color == RBTREE_RED)
  { // [CASE D5] 형제가 BLACK, 형제의 더 먼 자식이 RED
    right_rotate(t, sibling);
    exchange_color(sibling, parent);
    distant->color = RBTREE_BLACK;
    return;
  }

  // [CASE D2] 형제가 BLACK, 형제의 자식이 둘 다 BLACK
  sibling->color = RBTREE_RED;

  if (parent != t->root)
    rbtree_erase_fixup(t, parent->parent, parent->parent->left == parent);
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
  node_t *current = t->root;
  while(current->left != t->nil){
    current = current->left;
  }
  return current;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *current = t->root;
  while(current->right !=t->nil){
    current = current->right;
  }
  return current;
}

int rbtree_erase(rbtree *t, node_t *delete ) {
  // TODO: implement erase
  node_t *remove;// 트리에서 없어질 노드 ??? 그럼 p는 뭔뎅..;;
  node_t *remove_parent, *replace_node;
  int is_remove_black, is_remove_left;

  // step1 ) delete 삭제 후 대체할 replace node 찾기
  // step 1-1 delete 노드의 자식이 둘인 경우 :
  // delete 의 키를 후계자 노드의 키값으로 대체, 노드의 색은 delete의 색 유지;
  if(delete->left !=t->nil && delete->right !=t->nil){
    remove = get_next_node(t, delete); // 후계자 노드 (오른쪽 서브트리에서 가장 작은 노드)
    replace_node = remove->right;      // 대체할 노드: 지워질 노드인 후계자는 항상 왼쪽 자식이 없기 때문에, 자식이 있다면 오른쪽 자식 하나뿐임
    delete->key = remove->key;         // delete의 키를 후계자 노드의 키값으로 대체 (색은 변경 X)
  }else{
    remove = delete;
    replace_node = (remove->right != t->nil) ? remove->right : remove->left;
  }
  remove_parent = remove->parent;

  if (remove == t->root)
  {
    t->root = replace_node;        // 대체할 노드를 트리의 루트로 지정
    t->root->color = RBTREE_BLACK; // 루트 노드는 항상 BLACK
    free(remove);
    return 0; // 불균형 복구 함수 호출 불필요 (제거 전 트리에 노드가 하나 혹은 두개이므로 불균형이 발생하지 않음)
  }

  // Step 2-1) 'remove의 부모'와 'remove의 자식' 이어주기
  is_remove_black = remove->color; // remove 노드 제거 전에 지워진 노드의 색 저장
  is_remove_left = remove_parent->left == remove;

  // Step 2-1-1) 자식 연결
  if (is_remove_left) // remove가 왼쪽 자식이었을 경우: remove 부모의 왼쪽에 이어주기
    remove_parent->left = replace_node;
  else // remove가 오른쪽 자식이었을 경우: remove 부모의 오른쪽에 이어주기
    remove_parent->right = replace_node;

  // Step 2-1-2) 부모도 연결 (양방향 연결)
  replace_node->parent = remove_parent;
  free(remove);

  /* [CASE D2~D6]: remove 노드가 검정 노드인 경우 */
  // Step 3) 불균형 복구 함수 호출
  if (is_remove_black)
    rbtree_erase_fixup(t, remove_parent, is_remove_left);
  return 0;
}


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
