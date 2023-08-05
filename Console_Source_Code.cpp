#include <bits/stdc++.h>
#include <pqxx/pqxx> 

using namespace std;
using namespace pqxx;
#define precision(x,d) cout<<fixed<<setprecision(d)<<x

int main(int argc, char* argv[]) {
   string sql;
   connection C("dbname = 201701462 user = 201701462 password = meet@2301 \
      hostaddr = 10.100.71.21 port = 5432");
      if (C.is_open()) {
         cout << "Database: " << C.dbname() <<" is opened sucessfully"<< endl;
      } else {
         cout << "Can't open the database" << endl;
         return 1;
      }
    while(1)
    {
        loop: cout<<"Enter what you want to do"<<endl;
        cout<<"1. Find score card of perticular match"<<endl;
        cout<<"2. Find the points table before perticular match"<<endl;
        cout<<"3. Top 10 batsmen upto perticular match"<<endl;
        cout<<"4. Top 10 bowlers upto perticular match"<<endl;
	    cout<<"5. Find prjected score of perticular inning after a perticular over"<<endl;
	    cout<<"6. Top 10 batsmen in terms of sixes upto perticular match"<<endl;
	    cout<<"7. Top 10 batsmen in terms of fours upto perticular match"<<endl;
	    cout<<"8. TOP 10 highest individual scores upto perticular match"<<endl;
	    cout<<"9. Top 10 strike rate upto perticular match"<<endl;
	    cout<<"10. Top 10 Averages upto perticular match"<<endl;
	    cout<<"11. Top 10 bowlers in terms of average upto perticular match"<<endl;
	    cout<<"12. Top 10 bowlers in terms of economy upto perticular match"<<endl;
	    cout<<"13. Exit"<<endl;
        int choice;
        cin>>choice;
        if(choice==1)
        {
            int match;
	    cout<<"Choose the match from below given matches of the tournament"<<endl;
	    	try
		{
		    sql = "set search_path to worldcup;\ ";
		    sql+= "select match_no,team_1,team_2 from matches;";
		    nontransaction N(C);
		    result R2(N.exec(sql));
		    for (result::const_iterator c = R2.begin(); c != R2.end(); ++c)
		    {
		        cout<<c[0].as<int>()<<"  "<<c[1].as<string>()<<" vs "<<c[2].as<string>()<<endl;        
		    }
		}
		catch (const std::exception &e) 
		{
		        cerr << e.what() << std::endl;
			    goto loop;
		}	
            cin>>match;
            if(match>18)
            {
                cout<<"Please enter valid match"<<endl;
                goto loop;
            }
    		string a,b,team1,team2;
    		int x,cnt = 0;
		
            try 
            {
              /* Create SQL statement */
                sql.append("set search_path to worldcup;\ ");
                sql+="select team_id,PlayerName,run,ball,coalesce(fours, 0) as fours,coalesce(sixes, 0) as sixes,strike_rate from((select team_id,PlayerName,run,ball,fours,(100*(run::numeric/ball::numeric))::numeric as strike_rate from ((select team_id,PlayerName,run,ball,inning from ((select player_name as PlayerName,team_id from players) as a join";
                sql+="(select sum(runs) as run,count(runs) as ball,player_name,inning from ball_by_ball_batting where match_no=";
                sql+=to_string(match); 
                sql+=" and ball_event='l' group by player_name,inning) as b";
                sql+=" on a.PlayerName = b.player_name) as c) as d left join";
                sql+=" (select player_name,count(runs) as fours from ball_by_ball_batting where match_no=";
                sql+=to_string(match); 
                sql+="and ball_event='l' and runs=4 group by player_name) as e";
                sql+=" on d.PlayerName = e.player_name) as f) as g left join";  
                sql+=" (select player_name,count(runs) as sixes from ball_by_ball_batting where match_no=";
                sql+=to_string(match);
                sql+=" and ball_event='l' and runs=6 group by player_name) as h";
                sql+=" on g.PlayerName = h.player_name) as i; \ ";
                /* Create a non-transactional object. */
                nontransaction N(C);
              
                /* Execute SQL query */
                result R( N.exec( sql ));
	        	int flag = 0,f=1,print = 0;
                /* List down all the records */
                for (result::const_iterator c = R.begin(); c != R.end(); ++c)
                {
		    if(flag==0){
				flag = 1;
				team1 = c[0].as<string>();
				sql = "set search_path to worldcup;\ ";
				sql = "select find_inning(";
				sql+=to_string(match);
				sql+=",'";
				sql+=team1;
				sql+="'); \ ";
				result inn(N.exec(sql));
				for (result::const_iterator c = inn.begin(); c != inn.end(); ++c)
                		{
					if(c[0].as<int>()==2)
					{
						team2 = team1;
						team1 = "";
						f = 0;
					}
				}			
			}
			if(f==1)	
			{	
			    if(c[0].as<string>()==team1){	
					if(print==0){
					cout<<"For Team "<<team1<<" batting scorecard"<<endl<<endl;		
					    cout<<"Team"<<"   "<<"Player Name"<<"         "<<"Runs"<<"   "<<"Balls"<<"   "<<"4s"<<"   "<<"6s"<<"   "<<"Strike Rate"<<endl;
						print = 1;
					}
					a = c[0].as<string>();
					b = c[1].as<string>();
					    cout<<a;	
					for(int i=0;i<4-a.size();i++) cout<<" ";
					    cout<<"   "<<b;
					for(int i=0;i<20-b.size();i++) cout<<" ";
					x = c[2].as<int>();
					cout<<c[2].as<int>();
					cnt = to_string(x).size();
					
					for(int i=0;i<4-cnt;i++) cout<<" ";
					cout<<"   ";
					cout<<c[3].as<int>();
					x = c[3].as<int>();
					cnt = to_string(x).size();
					
					for(int i=0;i<5-cnt;i++) cout<<" ";
					cout<<"   ";
					cout<<c[4].as<int>();
					x = c[4].as<int>();
					cnt = to_string(x).size();
					
					for(int i=0;i<2-cnt;i++) cout<<" ";
					cout<<"   ";
					cout<<c[5].as<int>();
					x = c[5].as<int>();
					cnt = to_string(x).size();
					
					for(int i=0;i<2-cnt;i++) cout<<" ";
					cout<<"   ";
					precision(c[6].as<float>(),2);
					cout<<endl;
				}
			    else
				{
					team2 = c[0].as<string>();
				}
			}
			else
			{
			    if(c[0].as<string>()!=team2)
			    {
			        team1 = c[0].as<string>();
			        if(print==0){
					cout<<"For Team "<<team1<<" batting scorecard"<<endl<<endl;		
					    cout<<"Team"<<"   "<<"Player Name"<<"         "<<"Runs"<<"   "<<"Balls"<<"   "<<"4s"<<"   "<<"6s"<<"   "<<"Strike Rate"<<endl;
						print = 1;
					}
					a = c[0].as<string>();
					b = c[1].as<string>();
					    cout<<a;	
					for(int i=0;i<4-a.size();i++) cout<<" ";
					    cout<<"   "<<b;
					for(int i=0;i<20-b.size();i++) cout<<" ";
					x = c[2].as<int>();
					cout<<c[2].as<int>();
					cnt = to_string(x).size();
					
					for(int i=0;i<4-cnt;i++) cout<<" ";
					cout<<"   ";
					cout<<c[3].as<int>();
					x = c[3].as<int>();
					cnt = to_string(x).size();
					
					for(int i=0;i<5-cnt;i++) cout<<" ";
					cout<<"   ";
					cout<<c[4].as<int>();
					x = c[4].as<int>();
					cnt = to_string(x).size();
					
					for(int i=0;i<2-cnt;i++) cout<<" ";
					cout<<"   ";
					cout<<c[5].as<int>();
					x = c[5].as<int>();
					cnt = to_string(x).size();
					
					for(int i=0;i<2-cnt;i++) cout<<" ";
					cout<<"   ";
					precision(c[6].as<float>(),2);
					cout<<endl;
			    }
			}	

                }
		try
		    {
		        sql = "set search_path to worldcup; \ ";
		        sql+= "select team_id,PlayerName,run,ball,wickets,wides,no_balls,economy from (select PlayerName,coalesce(run, 0) as 					run,ball,coalesce(wickets, 0)";
		        sql+= " as wickets,coalesce(wides, 0) as wides,coalesce(no_balls, 0) as no_balls,(6*(run::numeric/		ball::numeric))::numeric as economy from((select PlayerName,team_id,run,wickets,ball from";
		        sql+= " ((select player_name as PlayerName,team_id,sum(runs) as run,count(*) as ball from (select * from ((select 				player_name,inning";
		        sql+= " ,team_id,over_no from ball_by_ball_bowling natural join players where match_no =";
		        sql+= to_string(match);
		        sql+=" )as a join (select ball_event,runs,inning,";
		        sql+= " over_no from ball_by_ball_batting where match_no=";
		        sql+= to_string(match);
		        sql+= ") as b on (a.inning=b.inning and a.over_no=b.over_no)) as c) as d";
	 	            sql+= " group by player_name,team_id) as m left join (select player_name,count(*) as wickets from ((select player_name,inning,over_no";
	 	            sql+= " from ball_by_ball_bowling where match_no = ";
	 	            sql+= to_string(match);
	 	            sql+= ") as a join	(select ball_event,runs,inning,over_no,ball_no from ball_by_ball_batting";
	 	            sql+= " where match_no=";
	 	            sql+= to_string(match);
	 	            sql+= ") as b on (a.inning=b.inning and a.over_no=b.over_no)) as c where ball_event='w' group by player_name) as n on";
	 	            sql+= " m.PlayerName = n.player_name) as o) as p left join (select player_name,count(*) as wides from ((select player_name,inning,over_no";
	 	            sql+= " from ball_by_ball_bowling where match_no = ";
	 	            sql+= to_string(match);
	 	            sql+= ") as a join (select ball_event,runs,inning,over_no,ball_no from ball_by_ball_batting where match_no=";
	 	            sql+= to_string(match);
	 	            sql+= ") as b on (a.inning=b.inning and a.over_no=b.over_no)) as c";
		        sql+= " where ball_event='wd' group by player_name) as q on p.PlayerName=q.player_name) as r left join (select player_name,count(*) as no_balls";
		        sql+= " from ((select player_name,inning,over_no from ball_by_ball_bowling where match_no = ";
		        sql+= to_string(match);
		        sql+= ") as a join (select ball_event,runs,inning,";
		        sql+= " over_no,ball_no from ball_by_ball_batting where match_no=";
		        sql+= to_string(match);
		        sql+= ") as b on (a.inning=b.inning and a.over_no=b.over_no)) as c";
		        sql+= " where ball_event='n' group by player_name) as s on r.PlayerName=s.player_name) as u join players as v on u.PlayerName=v.player_name; \ ";

			
		        /* Execute SQL query */
		        result R1( N.exec( sql ));
		        cout<<endl<<"FOR Team "<<team1<<" bowling scorecard"<<endl<<endl;  
			cout<<"Team"<<"   "<<"Player Name"<<"         "<<"Runs"<<"   "<<"Balls"<<"   "<<"wickets   "<<"wides   "
				<<"No_balls   "<<"Economy"<<endl;          
			for (result::const_iterator c = R1.begin(); c != R1.end(); ++c)
			{
			    if(c[0].as<string>()==team1){	
					a = c[0].as<string>();
					b = c[1].as<string>();
					    cout<<a;	
					for(int i=0;i<4-a.size();i++) cout<<" ";
					    cout<<"   "<<b;
					for(int i=0;i<20-b.size();i++) cout<<" ";
					x = c[2].as<int>();
					cout<<c[2].as<int>();
					cnt = to_string(x).size();
					
					for(int i=0;i<4-cnt;i++) cout<<" ";
					cout<<"   ";
					cout<<c[3].as<int>();
					x = c[3].as<int>();
					cnt = to_string(x).size();
					
					for(int i=0;i<5-cnt;i++) cout<<" ";
					cout<<"   ";
					cout<<c[4].as<int>();
					x = c[4].as<int>();
					cnt = to_string(x).size();
					
					for(int i=0;i<7-cnt;i++) cout<<" ";
					cout<<"   ";
					cout<<c[5].as<int>();
					x = c[5].as<int>();
					cnt = to_string(x).size();
					
					for(int i=0;i<5-cnt;i++) cout<<" ";
					x = c[6].as<int>();
					cout<<"   ";
					cout<<c[6].as<int>();
					cnt = to_string(x).size();

					for(int i=0;i<8-cnt;i++) cout<<" ";
					cout<<"   ";
					precision(c[7].as<float>(),2);
					cout<<endl;
				}
			}
			cout<<endl<<"For Team "<<team2<<" batting scorecard"<<endl<<endl;
			cout<<"Team"<<"   "<<"Player Name"<<"         "<<"Runs"<<"   "<<"Balls"<<"   "<<"4s"<<"   "<<"6s"
				<<"   "<<"Strike Rate"<<endl;

			for (result::const_iterator c = R.begin(); c != R.end(); ++c)
                	{
			   if(c[0].as<string>()==team2){
				a = c[0].as<string>();
				b = c[1].as<string>();
				    cout<<a;	
				for(int i=0;i<4-a.size();i++) cout<<" ";
				    cout<<"   "<<b;
				for(int i=0;i<20-b.size();i++) cout<<" ";
				x = c[2].as<int>();
				cout<<c[2].as<int>();
				cnt = to_string(x).size();
				
				for(int i=0;i<4-cnt;i++) cout<<" ";
				cout<<"   ";
			        cout<<c[3].as<int>();
			        x = c[3].as<int>();
				cnt = to_string(x).size();
				
				for(int i=0;i<5-cnt;i++) cout<<" ";
				cout<<"   ";
			        cout<<c[4].as<int>();
			        x = c[4].as<int>();
				cnt = to_string(x).size();
				
				for(int i=0;i<2-cnt;i++) cout<<" ";
				cout<<"   ";
			        cout<<c[5].as<int>();
			        x = c[5].as<int>();
				cnt = to_string(x).size();
				
				for(int i=0;i<2-cnt;i++) cout<<" ";
				cout<<"   ";
		        	precision(c[6].as<float>(),2);
		        	cout<<endl;
		
			    }
			}
			cout<<endl<<"For Team "<<team2<<" bowling scorecard"<<endl<<endl;
			cout<<"Team"<<"   "<<"Player Name"<<"         "<<"Runs"<<"   "<<"Balls"<<"   "<<"wickets   "<<"wides   "
				<<"No_balls   "<<"Economy"<<endl;          
			for (result::const_iterator c = R1.begin(); c != R1.end(); ++c)
			{
			    if(c[0].as<string>()==team2){	
					a = c[0].as<string>();
					b = c[1].as<string>();
					    cout<<a;	
					for(int i=0;i<4-a.size();i++) cout<<" ";
					    cout<<"   "<<b;
					for(int i=0;i<20-b.size();i++) cout<<" ";
					x = c[2].as<int>();
					cout<<c[2].as<int>();
					cnt = to_string(x).size();
					
					for(int i=0;i<4-cnt;i++) cout<<" ";
					cout<<"   ";
					cout<<c[3].as<int>();
					x = c[3].as<int>();
					cnt = to_string(x).size();
					
					for(int i=0;i<5-cnt;i++) cout<<" ";
					cout<<"   ";
					cout<<c[4].as<int>();
					x = c[4].as<int>();
					cnt = to_string(x).size();
					
					for(int i=0;i<7-cnt;i++) cout<<" ";
					cout<<"   ";
					cout<<c[5].as<int>();
					x = c[5].as<int>();
					cnt = to_string(x).size();
					
					for(int i=0;i<5-cnt;i++) cout<<" ";
					x = c[6].as<int>();
					cout<<"   ";
					cout<<c[6].as<int>();
					cnt = to_string(x).size();

					for(int i=0;i<8-cnt;i++) cout<<" ";
					cout<<"   ";
					precision(c[7].as<float>(),2);
					cout<<endl;
							    
				}
			}
		        
		}
		
    		catch (const std::exception &e) 
    		{
    		 cerr << e.what() << std::endl;
    		 goto loop;
    		}
	     }

            catch (const std::exception &e) 
            {
                cerr << e.what() << std::endl;
    	    goto loop;
            }
            
            try
            {
                sql = "set search_path to worldcup;\ ";
                sql+= " select team_id,sum(runs),inning as total from ball_by_ball_batting natural join players where match_no=";
                sql+= to_string(match);
                sql+= " group by team_id,inning;\ ";
                nontransaction N(C);
                result R3(N.exec(sql));
                sql = "set search_path to worldcup;\ ";
                sql+= "select team_id,sum(runs) as extras,inning from ball_by_ball_batting  natural join players where (ball_event = 'wd' or ball_event='lb' or ball_event='n') and match_no=";
                sql+= to_string(match);
                sql+= " group by team_id,inning;\ ";
                result R4(N.exec(sql));
                sql = "set search_path to worldcup;\ ";
                sql+= "select team_id,count(runs) as wickets,inning from ball_by_ball_batting  natural join players where (ball_event = 'w') and match_no=";
                sql+= to_string(match);
                sql+= " group by team_id,inning;\ ";
                result R5(N.exec(sql));
                cout<<"Team"<<"   "<<"Total"<<"   "<<"Wickets"<<"   "<<"Extras"<<endl;
                string ans1,ans2,t1,t2; 
        		int run1,run2,w1,w2;
                for (result::const_iterator c = R3.begin(); c != R3.end(); ++c)
			    {
			        if(c[2].as<int>()==1)
			        {
			            a = c[0].as<string>();
					    ans1 = a;	
					    for(int i=0;i<4-a.size();i++) ans1+=" ";
					    ans1+="   ";
					    ans1+=to_string(c[1].as<int>());
					    for(int i=0;i<5-to_string(c[1].as<int>()).size();i++) ans1+=" ";	
					    ans1+="   ";
					    run1 = c[1].as<int>();
					    t1 = a;	
			        }
			        else
			        {
			            a = c[0].as<string>();
					    ans2 = a;	
					    for(int i=0;i<4-a.size();i++) ans2+=" ";
					    ans2+="   ";
					    ans2+=to_string(c[1].as<int>());
					    for(int i=0;i<5-to_string(c[1].as<int>()).size();i++) ans2+=" ";	
					    ans2+="   ";
					    run2 = c[1].as<int>();
					    t2 = a;			
			        }
			    }
			    for (result::const_iterator c = R5.begin(); c != R5.end(); ++c)
			    {
			        if(c[2].as<int>()==1)
			        {
					    ans1+=to_string(c[1].as<int>());
					    for(int i=0;i<7-to_string(c[1].as<int>()).size();i++) ans1+=" ";	
					    w1 = (c[1].as<int>());				
					    ans1+="   ";
			        }
			        else
			        {
					    ans2+=to_string(c[1].as<int>());
					    for(int i=0;i<7-to_string(c[1].as<int>()).size();i++) ans2+=" ";	
					    w2 = (c[1].as<int>());
					    ans2+="   ";
			        }
			    }
			    for (result::const_iterator c = R4.begin(); c != R4.end(); ++c)
			    {
			        if(c[2].as<int>()==1)
			        {
					    ans1+=to_string(c[1].as<int>());
					    for(int i=0;i<6-to_string(c[1].as<int>()).size();i++) ans1+=" ";	
					    ans1+="   ";
			        }
			        else
			        {
					    ans2+=to_string(c[1].as<int>());
					    for(int i=0;i<6-to_string(c[1].as<int>()).size();i++) ans2+=" ";	
					    ans2+="   ";
			        }
			    }
			    cout<<ans1<<endl<<ans2<<endl;

			    cout<<"Result :";
				if(run1 > run2)
				{
				    cout<<t1<<" won by "<<run2-run1<<" runs"<<endl;
				}
                else if(run1 < run2)
                {
                    cout<<t2<<" won by "<<10-w2<<" wickets"<<endl;
                }
                else
                {
                    cout<<"Match tied"<<endl;
                }

            }
	        catch (const std::exception &e) 
            {
                cerr << e.what() << std::endl;
    	    goto loop;
            }	    
        }
        else if(choice==2)
        {
            try
            {
                nontransaction N(C);
                int match;
                cout<<"Enter the match number upto which you want to see the points table"<<endl;
                cin>>match;
                sql = "set search_path to worldcup;\ ";
                sql+= "select match_no,team_1,team_2 from matches where match_no <= ";
                sql+= to_string(match);
                result matches(N.exec(sql));
                map<string,int> wins,loses,ties,played;
                set<string> s;
                int cou = 1;
                for (result::const_iterator c = matches.begin(); c != matches.end(); ++c)
    			{
    			    string a,b;
    			    a = c[1].as<string>();
    			    b = c[2].as<string>();
    			    sql = "set search_path to worldcup;\ ";
    			    sql+= "select winner_of_match(";
    			    sql+=to_string(c[0].as<int>());
    			    sql+=");\ ";
                    result comp(N.exec(sql));
    			    for (result::const_iterator cc = comp.begin(); cc != comp.end(); ++cc)
    			    {
    			        if(cc[0].as<string>()==a)
    			        {
    			            wins[a]++;
    			            loses[b]++;
    			        }
    			        else if(cc[0].as<string>()==b)
    			        {
    			            wins[b]++;
    			            loses[a]++;
    			        }
    			        else
    			        {
    			            ties[a]++;
    			            ties[b]++;
    			        }
    			    }
    			    s.insert(a);
    			    s.insert(b);
    			    played[a]++;
    			    played[b]++;
    			}
    			vector<pair<int,string>> foo; 
    			for(auto it=s.begin();it!=s.end();it++)
    			{
    			    string a = *it;
    			    int points = 0;
    			    if(wins.find(a)!=wins.end())
    			    {
    			        points = wins[a]*2;
    			    }
    			    if(ties.find(a)!=ties.end())
    			    {
    			        points+=ties[a];
    			    }
    			    foo.push_back({points,a});
    			}
    			sort(foo.rbegin(),foo.rend());
    			cout<<"Team"<<"   "<<"Played"<<"   "<<"Won"<<"   "<<"Lost"<<"   "<<"Ties"<<"   "<<"Points"<<endl;  
    			for(int i=0;i<foo.size();i++)
    			{
    			    string a = foo[i].second;
                    
    			    cout<<a;
    			    for(int j=0;j<4-a.size();j++) cout<<" ";
    			    cout<<"   ";
    			    cout<<played[a];
    			    int x = played[a];
    			    for(int j=0;j<6-to_string(x).size();j++) cout<<" ";
    			    cout<<"   "<<wins[a];
    			    for(int j=0;j<3-to_string(wins[a]).size();j++) cout<<" ";
    			    cout<<"   "<<loses[a];
    			    for(int j=0;j<4-to_string(loses[a]).size();j++) cout<<" ";
    			    cout<<"   "<<ties[a];
    			    for(int j=0;j<4-to_string(ties[a]).size();j++) cout<<" ";
    			    cout<<"   "<<foo[i].first<<endl;
    			}
            }
            catch (const std::exception &e) 
            {
                cerr << e.what() << std::endl;
    	        goto loop;
            }
        }
        else if(choice==3)
        {
            int match;
            cout<<"Enter the match up to which you want to find top 10 batsmen"<<endl;
            cin>>match;
            nontransaction N(C);
            try
            {
                sql = "set search_path to worldcup;\ ";
                sql += "select PlayerName,team_id,run,(run::numeric/outs::numeric) as average,100*(run::numeric/balls::numeric) as strike_rate from (select PlayerName,run,balls,team_id,count(*) as";
                sql+=" outs from ((select player_name as PlayerName,team_id,sum(runs) as run,count(runs) as balls from ball_by_ball_batting  					natural join players where match_no <=";
                sql+=to_string(match); 
		        sql+=" group by player_name,team_id ) as a left join (select player_name from ball_by_ball_batting where match_no <=";
		        sql+=to_string(match);
		        sql+=" and ball_event = 'w') as b on a.PlayerName=b.player_name) as c group by PlayerName,run,balls,team_id) as d order by run desc limit 10;\ ";
		        
                result top_10(N.exec(sql));
                cout<<"Player Name"<<"         "<<"Team"<<"   "<<"Runs"<<"   "<<"Average"<<"   "<<"Strike Rate"<<endl;
                for (result::const_iterator c = top_10.begin(); c != top_10.end(); ++c)
    		{
    			    string a = c[0].as<string>();
				cout<<a;	
			   for(int i=0;i<20-a.size();i++) cout<<" ";
		            cout<<c[1].as<string>();
		            for(int i=0;i<4-c[1].as<string>().size();i++) cout<<" ";
		            cout<<"   ";
		            cout<<c[2].as<int>();
		            int x = c[2].as<int>();
				for(int i=0;i<4-to_string(x).size();i++) cout<<" ";
                    		cout<<"   ";
                    	precision(c[3].as<float>(),2);
			cout<<"    ";
                    	precision(c[4].as<float>(),2);
			cout<<endl;
    		}
    			
            }
            catch (const std::exception &e) 
            {
                cerr << e.what() << std::endl;
    	        goto loop;
            }
        }
        else if(choice==4)
        {
            int match;
            cout<<"Enter the match up to which you want to find top 10 bowlers"<<endl;
            cin>>match;
            nontransaction N(C);
            try
            {
                sql = "set search_path to worldcup;\ ";
                sql+="select bowler,team_id,wickets,(balls::numeric/wickets::numeric) as average,6*(runs::numeric/balls::numeric) as economy from"; 
                sql+=" ((select bowler,count(*) as wickets,team_id from (select a.player_name as bowler,b.player_name as batsman,team_id from (ball_by_ball_bowling natural join players) as";
                sql+=" a join ball_by_ball_batting as b on (a.inning =  b.inning and a.over_no=b.over_no and a.match_no=b.match_no) where a.match_no<=";
                sql+=to_string(match);
                sql+=" and b.ball_event='w' ) as c group by bowler,team_id) as d join (select a.player_name,count(*) as balls,sum(runs) as runs from";
                sql+=" (ball_by_ball_bowling natural join players) as a join ball_by_ball_batting as b on (a.inning =  b.inning and a.over_no=b.over_no and a.match_no=b.match_no)";
                sql+=" where a.match_no<=";
                sql+=to_string(18);
                sql+=" group by a.player_name) as e on d.bowler = e.player_name) as f order by wickets desc limit 10;\ ";
                result top_10(N.exec(sql));
                cout<<"Player Name"<<"         "<<"Team"<<"   "<<"Wickets"<<"   "<<"Average"<<"   "<<"Economy"<<endl;
                for (result::const_iterator c = top_10.begin(); c != top_10.end(); ++c)
    		{
    			    string a = c[0].as<string>();
				cout<<a;	
    			    for(int i=0;i<20-a.size();i++) cout<<" ";
    		        cout<<c[1].as<string>();
		            for(int i=0;i<4-c[1].as<string>().size();i++) cout<<" ";
		            cout<<"   ";
		            cout<<c[2].as<int>();
		            int x = c[2].as<int>();
				    for(int i=0;i<7-to_string(x).size();i++) cout<<" ";
                    cout<<"   ";
                    precision(c[3].as<float>(),2);
                    cout<<"     ";
                    precision(c[4].as<float>(),2);
                    cout<<endl;
    		}
    			
            }
            catch (const std::exception &e) 
            {
                cerr << e.what() << std::endl;
    	        goto loop;
            }
        }
        else if(choice==5)
        {
            int match;
            cout<<"Following are tha matches chose one of them"<<endl;
            try
    		{
    		    sql = "set search_path to worldcup;\ ";
    		    sql+= "select match_no,team_1,team_2 from matches;\ ";
    		    nontransaction N(C);
    		    result RR(N.exec(sql));
    		    for (result::const_iterator c = RR.begin(); c != RR.end(); ++c)
    		    {
    		        cout<<c[0].as<int>()<<"  "<<c[1].as<string>()<<" vs "<<c[2].as<string>()<<endl;        
    		    }
    		    cin>>match;
    		    string team1,team2;
    		    for (result::const_iterator c = RR.begin(); c != RR.end(); ++c)
    		    {
    		        if(c[0].as<int>()==match)
    		        {
    		            team1 = c[1].as<string>();
    		            team2 = c[2].as<string>();
    		        }
    		    }
    		    sql = "set search_path to worldcup;\ ";
    		    sql+= "select find_inning(";
    		    sql+= to_string(match);
    		    sql+= ",'";
    		    sql+=team1;
    		    sql+="');\ ";
    		    result r(N.exec(sql));
    		    string team;
    		    for (result::const_iterator c = r.begin(); c != r.end(); ++c)
    		    {
    		        if(c[0].as<int>()==1)
    		        {
    		            team = team1;
    		        }
    		        else
    		        {
    		            team = team2;
    		        }
    		    }
    		    cout<<"In match "<<match<<" "<<team<<" played the first inning and choose over from ";
    		    sql = "set search_path to worldcup;\ ";
    		    sql = " select distinct over_no from ball_by_ball_batting where match_no = ";
    		    sql+= to_string(match);
    		    sql+= " and inning = ";
    		    sql+= to_string(1); 
    		    sql+= " order by over_no;\ ";
    		    result rr(N.exec(sql));
    		    int mn = 100,mx = -1;
    		    for (result::const_iterator c = rr.begin(); c != rr.end(); ++c)
    		    {
    		        mn = min(mn,c[0].as<int>());
    		        mx = max(mx,c[0].as<int>());
    		    }
    		    cout<<mn<<" to "<<mx<<" that they have batted"<<endl;
    		    int over;
    		    cin>>over;
    		    sql = "set search_path to worldcup;\ ";
    		    sql+= "select run,(run::numeric/5::numeric) as crr from (select sum(runs) as run from ball_by_ball_batting where match_no = ";
    		    sql+=to_string(match);
    		    sql+= " and inning = 1 and over_no <= ";
    		    sql+=to_string(over);
    		    sql+= ") as a;\ ";
                result cr(N.exec(sql));
                float crr;
                int runs;
                for (result::const_iterator c = cr.begin(); c != cr.end(); ++c)
    		    {
    		        runs = c[0].as<int>();
    		        crr = c[1].as<float>();
    		    }
    		    cout<<"Team"<<"   "<<"At current rpo"<<"  "<<"At 6 rpo  "<<"At 8 rpo  "<<"At 10 rpo  "<<endl;
    		    cout<<team;
    		    for(int i=0;i<4-team.size();i++) cout<<" ";
    		    cout<<"   "<<runs + floor((float)(crr)*(float(10.0- over)))<<"             ";
    		    cout<<runs + 6*(10-over)<<"        ";
    		    cout<<runs + 8*(10-over)<<"       ";
    		    cout<<runs + 10*(10-over)<<endl;
    		}
    		catch (const std::exception &e) 
    		{
    		        cerr << e.what() << std::endl;
    			    goto loop;
    		}
        }
        else if(choice==6)
        {
            int match;
            cout<<"Enter match upto which you want to see top 10 batmen in terms of sixes"<<endl;
            cin>>match;
            try
            {
                sql = "set search_path to worldcup;\ ";
                sql+="select player_name,count(*) as sixes from ball_by_ball_batting where match_no<=";
                sql+= to_string(match);
                sql+=" and ball_event='l' and runs = 6 group by player_name	order by sixes desc limit 10;\ ";
		nontransaction N(C);
                result sixes(N.exec(sql));
                cout<<"Player Name         "<<"Sixes"<<endl;
                for(result::const_iterator c = sixes.begin();c!=sixes.end();++c)
                {
                    cout<<c[0].as<string>();
                    for(int i=0;i<20-c[0].as<string>().size();i++)
                        cout<<" ";
                    cout<<c[1].as<int>()<<endl;
                }
            }
    		catch (const std::exception &e) 
    		{
    		        cerr << e.what() << std::endl;
    			    goto loop;
    		}
        }
        else if(choice ==7)
        {
            int match;
            cout<<"Enter match upto which you want to see top 10 batmen in terms of fours"<<endl;
            cin>>match;
            try
            {
                sql = "set search_path to worldcup;\ ";
                sql+="select player_name,count(*) as fours from ball_by_ball_batting where match_no<=";
                sql+= to_string(match);
                sql+=" and ball_event='l' and runs = 4 group by player_name order by fours desc limit 10;\ ";
		        nontransaction N(C);
                result fours(N.exec(sql));
                cout<<"Player Name         "<<"Fours"<<endl;
                for(result::const_iterator c = fours.begin();c!=fours.end();++c)
                {
                    cout<<c[0].as<string>();
                    for(int i=0;i<20-c[0].as<string>().size();i++)
                        cout<<" ";
                    cout<<c[1].as<int>()<<endl;
                }
            }
    		catch (const std::exception &e) 
    		{
    		        cerr << e.what() << std::endl;
    			    goto loop;
    		}
        }
        else if(choice==8)
        {
            int match;
            cout<<"Enter match upto which you want to see top 10 individual scores"<<endl;
            cin>>match;
            try
            {
                sql = "set search_path to worldcup;\ ";
                sql+="select player_name,team_id,match_no,sum(runs) as runs from (ball_by_ball_batting natural join players) where match_no<=";
                sql+= to_string(match);
                sql+=" and ball_event='l' group by player_name,match_no,team_id	order by runs desc limit 10;\ ";
		nontransaction N(C);
                result best(N.exec(sql));
                cout<<"Player Name         "<<"Team"<<"   "<<"Match No"<<"   "<<"runs"<<endl;
                for(result::const_iterator c = best.begin();c!=best.end();++c)
                {
                    cout<<c[0].as<string>();
                    for(int i=0;i<20-c[0].as<string>().size();i++)
                        cout<<" ";
                    cout<<c[1].as<string>();
                    for(int i=0;i<4-c[1].as<string>().size();i++)
                        cout<<" ";
                    cout<<"   ";
                    cout<<c[2].as<int>();
                    for(int i=0;i<8-c[2].as<string>().size();i++)
                        cout<<" ";
                    cout<<"   "<<c[3].as<int>()<<endl;
                }
            }
    		catch (const std::exception &e) 
    		{
    		        cerr << e.what() << std::endl;
    			    goto loop;
    		}
        }
        else if(choice==9)
        {
		    int match;
            cout<<"Enter match upto which you want to see top 10 strike rates"<<endl;
            cin>>match;
            
            try
            {			
                sql = "set search_path to worldcup;\ ";
                sql+="select player_name,team_id,100*(runs::numeric/balls::numeric) as strike_rate from";
                sql+= " (select player_name,sum(runs) as runs, count(runs) as balls,team_id from (ball_by_ball_batting natural join players)";
                sql+= " where match_no<=";
                sql+= to_string(match);
                sql+= " and ball_event='l' group by player_name,team_id) as a order by strike_rate desc limit 10;\ ";
		        nontransaction N(C);
                result sr(N.exec(sql));
                cout<<"Player Name         "<<"Team"<<"   "<<"Strike Rate"<<endl;
                for(result::const_iterator c = sr.begin();c!=sr.end();++c)
                {
                    cout<<c[0].as<string>();
                    for(int i=0;i<20-c[0].as<string>().size();i++)
                        cout<<" ";
                    cout<<c[1].as<string>();    
                    for(int i=0;i<4-c[1].as<string>().size();i++)
                        cout<<" ";
			cout<<"   ";
                    precision(c[2].as<float>(),2);  
			cout<<endl;  
                }
            }
    		catch (const std::exception &e) 
    		{
    		        cerr << e.what() << std::endl;
    			    goto loop;
    		}            
        }
        else if(choice==10)
        {
            int match;
            cout<<"Enter match upto which you want to see top 10 Averages "<<endl;
            cin>>match;
            
            try
            {			
                sql = "set search_path to worldcup;\ ";
                sql += "select PlayerName,team_id,(run::numeric/outs::numeric) as average,run,100*(run::numeric/balls::numeric) as strike_rate from (select PlayerName,run,balls,team_id,count(*) as";
                sql+=" outs from ((select player_name as PlayerName,team_id,sum(runs) as run,count(runs) as balls from ball_by_ball_batting  					natural join players where match_no <=";
                sql+=to_string(match); 
		        sql+=" group by player_name,team_id ) as a left join (select player_name from ball_by_ball_batting where match_no <=";
		        sql+=to_string(match);
		        sql+=" and ball_event = 'w') as b on a.PlayerName=b.player_name) as c group by PlayerName,run,balls,team_id) as d order by average desc limit 10;\ ";
		        
                nontransaction N(C);
                result avg(N.exec(sql));
                cout<<"Player Name         "<<"Team"<<"   "<<"Averages"<<endl;
                for(result::const_iterator c = avg.begin();c!=avg.end();++c)
                {
                    cout<<c[0].as<string>();
                    for(int i=0;i<20-c[0].as<string>().size();i++)
                        cout<<" ";
                    cout<<c[1].as<string>();    
                    for(int i=0;i<4-c[1].as<string>().size();i++)
                        cout<<" ";
			cout<<"   ";
                    precision(c[2].as<float>(),2);  
			cout<<endl;  
                }
            }
    		catch (const std::exception &e) 
    		{
    		        cerr << e.what() << std::endl;
    			    goto loop;
    		}            

        }
        else if(choice==11)
        {
            int match;
            cout<<"Enter the match up to which you want to find top 10 bowlers in terms of average"<<endl;
            cin>>match;
            nontransaction N(C);
            try
            {
                sql = "set search_path to worldcup;\ ";
                sql+="select bowler,team_id,(balls::numeric/wickets::numeric) as average,wickets,6*(runs::numeric/balls::numeric) as economy from"; 
                sql+=" ((select bowler,count(*) as wickets,team_id from (select a.player_name as bowler,b.player_name as batsman,team_id from (ball_by_ball_bowling natural join players) as";
                sql+=" a join ball_by_ball_batting as b on (a.inning =  b.inning and a.over_no=b.over_no and a.match_no=b.match_no) where a.match_no<=";
                sql+=to_string(match);
                sql+=" and b.ball_event='w' ) as c group by bowler,team_id) as d join (select a.player_name,count(*) as balls,sum(runs) as runs from";
                sql+=" (ball_by_ball_bowling natural join players) as a join ball_by_ball_batting as b on (a.inning =  b.inning and a.over_no=b.over_no and a.match_no=b.match_no)";
                sql+=" where a.match_no<=";
                sql+=to_string(18);
                sql+=" group by a.player_name) as e on d.bowler = e.player_name) as f order by average limit 10;\ ";
                result topp_10(N.exec(sql));
                cout<<"Player Name         "<<"Team"<<"   "<<"Averages"<<endl;
                for(result::const_iterator c = topp_10.begin();c!=topp_10.end();++c)
                {
                    cout<<c[0].as<string>();
                    for(int i=0;i<20-c[0].as<string>().size();i++)
                        cout<<" ";
                    cout<<c[1].as<string>();    
                    for(int i=0;i<4-c[1].as<string>().size();i++)
                        cout<<" ";
			cout<<"   ";
                    precision(c[2].as<float>(),2);  
			cout<<endl;  
                }
    			
            }
            catch (const std::exception &e) 
    		{
    		        cerr << e.what() << std::endl;
    			    goto loop;
    		}
        }
        else if(choice==12)
        {
                int match;
                cout<<"Enter the match up to which you want to find top 10 bowlers in terms of economy"<<endl;
                cin>>match;
                nontransaction N(C);
                try
                {
                    sql = "set search_path to worldcup;\ ";
                    sql+="select bowler,team_id,6*(runs::numeric/balls::numeric) as economy,(balls::numeric/wickets::numeric) as average,wickets from"; 
                    sql+=" ((select bowler,count(*) as wickets,team_id from (select a.player_name as bowler,b.player_name as batsman,team_id from (ball_by_ball_bowling natural join players) as";
                    sql+=" a join ball_by_ball_batting as b on (a.inning =  b.inning and a.over_no=b.over_no and a.match_no=b.match_no) where a.match_no<=";
                    sql+=to_string(match);
                    sql+=" and b.ball_event='w' ) as c group by bowler,team_id) as d join (select a.player_name,count(*) as balls,sum(runs) as runs from";
                    sql+=" (ball_by_ball_bowling natural join players) as a join ball_by_ball_batting as b on (a.inning =  b.inning and a.over_no=b.over_no and a.match_no=b.match_no)";
                    sql+=" where a.match_no<=";
                    sql+=to_string(18);
                    sql+=" group by a.player_name) as e on d.bowler = e.player_name) as f order by economy limit 10;\ ";
                    result topp_10(N.exec(sql));
                    cout<<"Player Name         "<<"Team"<<"   "<<"Economy"<<endl;
                    for(result::const_iterator c = topp_10.begin();c!=topp_10.end();++c)
                    {
                        cout<<c[0].as<string>();
                        for(int i=0;i<20-c[0].as<string>().size();i++)
                            cout<<" ";
                        cout<<c[1].as<string>();    
                        for(int i=0;i<4-c[1].as<string>().size();i++)
                            cout<<" ";
    			cout<<"   ";
                        precision(c[2].as<float>(),2);  
    			cout<<endl;  
                    }
        			
            }
            
            catch (const std::exception &e) 
            {
                cerr << e.what() << std::endl;
    	        goto loop;
            }
            
        }
        else
        {
    	    C.disconnect(); 
    	    break;
        }
    }

   return 0;
}
