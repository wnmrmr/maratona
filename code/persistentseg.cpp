struct PersistentSeg {
	struct Node {
		Node *lchild, *rchild;
		int data;
		Node() {
			lchild = rchild = NULL;
			data = 0;
		}
	};
	int size;
	vector<Node*> root;
	PersistentSeg(int _size, int v[]) {
		size = _size;
		root.push_back(new Node);	
		build(root[0],1,size,v);
	}
	void build(Node* cur, int nl, int nr, int v[]) {
		if(nl == nr) {
			cur->data = v[nl];
			return;
		}
		int mid = (nl+nr)/2;
		cur->lchild = new Node;
		cur->rchild = new Node;
		build(cur->lchild,nl,mid,v); 
		build(cur->rchild,mid+1,nr,v);
		cur->data = cur->lchild->data + cur->rchild->data;
	}
	void upd(Node* cur, Node* prev, int nl, int nr, int pos, int val) {
		if(nl == nr) {
			cur->data = prev->data + val;
			return;
		}
		int mid = (nl+nr)/2;
		if(pos <= mid) {
			cur->lchild = new Node;
			cur->rchild = prev->rchild;
			upd(cur->lchild,prev->lchild,nl,mid,pos,val);
		} else {
			cur->rchild = new Node;
			cur->lchild = prev->lchild;
			upd(cur->rchild,prev->rchild,mid+1,nr,pos,val);
		}
		cur->data = cur->lchild->data + cur->rchild->data;
	}
	void update(int pos, int val) {
		Node* prev = root.back();	
		root.push_back(new Node);
		upd(root.back(),prev,1,size,pos,val);
	}
	int qry(Node* cur, int nl, int nr, int ql, int qr) {
		if(nr < ql || qr < nl) {
			return 0;
		}
		if(ql <= nl && nr <= qr) {
			return cur->data;
		}
		int mid = (nl+nr)/2;
		return qry(cur->lchild,nl,mid,ql,qr) + qry(cur->rchild,mid+1,nr,ql,qr);
	}
	int query(int l, int r, int time) {
		return qry(root[time],1,size,l,r);
	}
}
