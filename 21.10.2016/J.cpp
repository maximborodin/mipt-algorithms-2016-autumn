#include<cstdio>
#include<cstring>
#include<algorithm>

int Ans,T(2),x[11000],y[11000],z[11000],r[11000],f[11000],F[11000],
	s[1100000],b[11000],n,m,t[11000],c[11000],K,q[11000],Q[11000];

void In(int a,int b,int c,int d)
{   x[T]=f[a];y[T]=b;z[T]=c;r[T]=d;f[a]=T++;
    x[T]=f[b];y[T]=a;z[T]=0;r[T]=-d;f[b]=T++;
}

int main()
{   int i,j,k;
//	freopen("c.in","rb",stdin);
//	freopen("c.out","wb",stdout);
	scanf("%d%d",&n,&K);
	for(i=0;i<n;i++)
	{	scanf("%d%d%d",s+i,t+i,c+i);
		t[i]+=s[i];
		q[m++]=s[i];
		q[m++]=t[i];
	}
	q[m++]=-1;
	std::sort(q,q+m);
	for(i=j=1;i<m;i++)if(q[i]!=q[j-1])q[j++]=q[i];
	m=j;
	for(i=0;i+1<m;i++)In(i,i+1,K,0);
	for(i=0;i<n;i++)
	{	for(j=0;j<m;j++)if(q[j]==s[i])break;
		s[i]=j;
		for(j=0;j<m;j++)if(q[j]==t[i])break;
		t[i]=j;
		Q[i]=T;
        In(s[i],t[i],1,-c[i]);
	}
		s[0]=0;
	for(;;)
	{	memset(b,127,sizeof(b));
		b[0]=0;
		for(i=0,j=1;i<j;i++)
			for(k=f[s[i]];k;k=x[k])
				if(z[k]&&b[s[i]]+r[k]<b[y[k]])
				{	b[y[k]]=b[s[i]]+r[k];
					F[y[k]]=k;
					s[j++]=y[k];
				}
		if(b[m-1]==2139062143)break;
		j=2139062143;
		for(i=m-1;i;i=y[F[i]^1])if(j>z[F[i]])j=z[F[i]];
		for(i=m-1;i;i=y[F[i]^1])
		{	Ans+=j*r[F[i]];
			z[F[i]]-=j;
			z[F[i]^1]+=j;
		}
	}
    for(i=0;i<n;i++)if(z[Q[i]])printf("0 ");else printf("1 ");printf("\n");
    return 0;
}
