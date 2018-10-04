/*input
Greedily we have to reduce the first biggest c values and subtract 1 from them and sort again
sort again part can be saved by some smart thinking
say i have 8 8 8 7 7 7 6 6 6 5 and I have to reduce first 7 values
so it becomes 7 7 7 6 6 6 5 6 6 5 and then sort it
instead of reducing first 6 i reduce last 6
so it becomes 7 7 7 6 6 6 6 6 5 5

so first of all sort the array in descending order
now we need to do the following for each operation
find the value at cth position. Let it be val
then find first occurance and last occurance of val
we can reduce the range from 1 to first - 1 
and then we need to find how many val elements we need to reduce
since we have reduced first - 1 elements we need to reduce c - (first - 1) more elements
since we do this at the end, so we decrement range from lastpos - (c - (first - 1)) + 1 to last
*/
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100005;

int n, q;

struct node{
	int mx, mn, lazy;
};

struct node st[4*MAXN];
int arr[MAXN];

void build(int node, int be, int en){
	if(be == en){
		st[node].mx = arr[be];
		st[node].mn = arr[be];
		st[node].lazy = 0;
		return;
	}
	int mid = (be + en) >> 1;
	build(2*node, be, mid);
	build(2*node + 1, mid + 1, en);
	st[node].mx = max(st[2*node].mx, st[2*node+1].mx);
	st[node].mn = min(st[2*node].mn, st[2*node+1].mn);
	st[node].lazy = st[2*node].lazy + st[2*node + 1].lazy;
}

void propagate(int node, int be, int en){
	st[node].mx -= st[node].lazy;
	st[node].mn -= st[node].lazy;
	if(be != en){
		st[2*node].lazy += st[node].lazy;
		st[2*node+1].lazy += st[node].lazy;
	}
	st[node].lazy = 0;
}

int findatpos(int node, int be, int en, int pos){
	if(st[node].lazy){
		propagate(node, be, en);
	}
	if(be == en){
		return st[node].mx; //or equivalently st[node].mn
	}
	int mid = (be + en) >> 1;
	if(pos <= mid)
		return findatpos(2*node, be, mid, pos);
	else
		return findatpos(2*node + 1, mid + 1, en, pos);
}

//return -1 if doesnt exist
int findfirst(int node, int be, int en, int val){
	if(st[node].lazy){
		propagate(node, be, en);
	}
	int mid = (be + en) >> 1;
	if(be != en){
		if(st[2*node].lazy){
			propagate(2*node, be, mid);
		}
		if(st[2*node+1].lazy){
			propagate(2*node + 1, mid + 1, en);
		}
	}

	if(be == en){
		if(st[node].mx == val)return be;
		else return -1;
	}


	//check if left node mein lie kar sakta hai
	if(val <= st[2*node].mx && val >= st[2*node].mn)return findfirst(2*node, be, mid, val);		
	if(val <= st[2*node+1].mx && val >= st[2*node+1].mn)return findfirst(2*node + 1, mid + 1, en, val);
	return -1;
}

//return -1 if doesnt exist
int findlast(int node, int be, int en, int val){
	if(st[node].lazy){
		propagate(node, be, en);
	}
	int mid = (be + en) >> 1;
	if(be != en){
		if(st[2*node].lazy){
			propagate(2*node, be, mid);
		}
		if(st[2*node+1].lazy){
			propagate(2*node + 1, mid + 1, en);
		}
	}

	if(be == en){
		if(st[node].mx == val)return be;
		else return -1;
	}



	//check if right node mein lie kar sakta hai
	if(val <= st[2*node+1].mx && val >= st[2*node+1].mn)return findlast(2*node + 1, mid + 1, en, val);
	if(val <= st[2*node].mx && val >= st[2*node].mn)return findlast(2*node, be, mid, val);		
	return -1;
}

void decrement(int node, int be, int en, int l, int r){
	if(st[node].lazy){
		propagate(node, be, en);
	}
	if(be > r || en < l)return;
	if(be >= l && en <= r){
		st[node].mx--;
		st[node].mn--;
		if(be != en){
			st[2*node].lazy++;
			st[2*node+1].lazy++;
		}
		return;
	}
	int mid = (be + en) >> 1;
	decrement(2*node, be, mid, l, r);
	decrement(2*node + 1, mid + 1, en, l, r);
	st[node].mx = max(st[2*node].mx, st[2*node+1].mx);
	st[node].mn = min(st[2*node].mn, st[2*node+1].mn);
	st[node].lazy = st[2*node].lazy + st[2*node+1].lazy;
}

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);

	int x;
	
	cin >> n >> q;

	for(int i = 1; i <= n; ++i)cin >> arr[i];

	sort(arr + 1, arr + n + 1, greater<int>());

	build(1, 1, n);

	int ctr = 0;

	while(q--){
		cin >> x;
		if(x > n)break;
		int val = findatpos(1, 1, n, x);
		if(val == 0)break;
		ctr++;
		int firstpos = findfirst(1, 1, n, val);
		int lastpos = findlast(1, 1, n, val);
		decrement(1, 1, n, 1, firstpos - 1);
		decrement(1, 1, n, lastpos - (x - (firstpos - 1)) + 1, lastpos);
	}

	cout << ctr << endl;

	return 0;
}
