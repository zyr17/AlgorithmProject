#include <bits/stdc++.h>
int p[4][2] = {{0,0},{0,-1},{-1,0},{-1,-1}};
int d[10101][10101];
char fa[10101][10101];
char s1[10101], s2[10101];
int n, m;
int main(){
	freopen("task1.in","r",stdin);
	freopen("task1.out","w",stdout);
	scanf("%s%s", s1 + 1, s2 + 1);
	n = strlen(s1 + 1);
	m = strlen(s2 + 1);
	memset(d, 63, sizeof d);
	for (int i = 0; i <= n; i ++ ){
		d[i][0] = i;
		fa[i][0] = 2;
	}
	for (int j = 0; j <= m; j ++ ){
		d[0][j] = j;
		fa[0][j] = 1;
	}
	fa[0][0] = 0;
	for (int i = 1; i <= n; i ++ )
		for (int j = 1; j <= m; j ++ ){
			if (d[i][j] > d[i - 1][j - 1] + (s1[i] != s2[j])){
				d[i][j] = d[i - 1][j - 1] + (s1[i] != s2[j]);
				fa[i][j] = 3;
			}
			if (d[i][j] > d[i - 1][j] + 1){
				d[i][j] = d[i - 1][j] + 1;
				fa[i][j] = 2;
			}
			if (d[i][j] > d[i][j - 1] + 1){
				d[i][j] = d[i][j - 1] + 1;
				fa[i][j] = 1;
			}
		}
	printf("%d\n", d[n][m]);
	std::vector<std::string> ans;
	for (; fa[n][m]; ){
		int t = fa[n][m];
		char buf[100];
		if (t == 3){
			if (s1[n] != s2[m])
				sprintf(buf, "SUB %d %c\n", n - 1, s2[m]);
			else buf[0] = 0;
		}
		else if (t == 2){
			sprintf(buf, "DEL %d\n", n - 1);
		}
		else if (t == 1){
			sprintf(buf, "INS %d %c\n", n, s2[m]);
		}
		if (buf[0]) ans.push_back(buf);
		n += p[t][0];
		m += p[t][1];
	}
	for (int i = ans.size() - 1; i >= 0; i -- )
		printf("%s", ans[i].c_str());
}
