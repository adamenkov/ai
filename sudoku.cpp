#include <iostream>
#include <set>
#include <vector>

using namespace std;


class SudokuSolver
{
	struct Variable
	{
		int x;
		vector<int> domain;
		
		friend ostream& operator<<(ostream& os, const Variable& v)
		{
			return os << v.x;
		}
	};
	
	Variable v[9][9];


public:
	SudokuSolver(char board[9][10])
	{
		for (int i = 0; i < 9; ++i)
		{
			for (int j = 0; j < 9; ++j)
			{
				char x = board[i][j];
				std::vector<int>& d = v[i][j].domain;
				
				if (x == ' ')
				{
					for (int k = 1; k <= 9; ++k)
					{
						d.push_back(k);
					}
				}
				else
				{
					d.push_back(x - '0');
				}
			}
		}
		
		Inference();
	}
	
	
	void Inference()
	{
		bool changed;
		do
		{
			changed = false;
					
			for (int i = 0; i < 9; ++i)
			{
				for (int j = 0; j < 9; ++j)
				{
					vector<int>& d = v[i][j].domain;
					if (d.size() > 1)
					{
						for (int digit : d)
						{
							bool unique = true;
							
							for (int c = 0; c < 9; ++c)
							{
								if (c == j) continue;
								
								vector<int>& d2 = v[i][c].domain;
								if (find(d2.begin(), d2.end(), digit) != d2.end())
								{
									unique = false;
									break;
								}
							}
							
							if (unique)
							{
								d.clear();
								d.push_back(digit);
								changed = true;
								break;
							}

							unique = true;
							
							for (int r = 0; r < 9; ++r)
							{
								if (r == i) continue;
								
								vector<int>& d2 = v[r][j].domain;
								if (find(d2.begin(), d2.end(), digit) != d2.end())
								{
									unique = false;
									break;
								}
							}
							
							if (unique)
							{
								d.clear();
								d.push_back(digit);
								changed = true;
								break;
							}

							unique = true;
							
							int I = i / 3 * 3;
							int J = j / 3 * 3;
							for (int i2 = I; i2 < I + 3; ++i2)
							{
								for (int j2 = J; j2 < J + 3; ++j2)
								{
									if ((i2 == i) && (j2 == j)) continue;
									
									vector<int>& d2 = v[i2][j2].domain;
									if (find(d2.begin(), d2.end(), digit) != d2.end())
									{
										unique = false;
										break;
									}
								}
							}
							
							if (unique)
							{
								d.clear();
								d.push_back(digit);
								changed = true;
								break;
							}
						}
					}
					
					for (int i2 = 0; i2 < 9; ++i2)
					{
						for (int j2 = 0; j2 < 9; ++j2)
						{
							if ((i2 == i) && (j2 == j))
								continue;
							
							if ((i2 != i) && (j2 != j) && !((i2 / 3 == i / 3) && (j2 / 3 == j / 3)))
								continue;
								
							vector<int>& d2 = v[i2][j2].domain;
							switch (d2.size())
							{
								case 1:
								{
									int digit2 = d2.front();
									changed |= try_remove(d, digit2);
									break;
								}
								case 2:
								case 3:
								{
									for (int i3 = 0; i3 < 9; ++i3)
									{
										for (int j3 = 0; j3 < 9; ++j3)
										{
											if ((i3 == i) && (j3 == j))
												continue;
											
											if ((i3 == i2) && (j3 == j2))
												continue;
											
											if (((i == i2) && (i2 == i3)) ||
												((j == j2) && (j2 == j3)) ||
												((i / 3 == i2 / 3) && (i2 / 3 == i3 / 3) && (j / 3 == j2 / 3) && (j2 / 3 == j3 / 3)))
											{
												vector<int>& d3 = v[i3][j3].domain;
												if ((d2.size() == 2) && (d3.size() == 2) && (d3 == d2))
												{
													changed |= try_remove(d, d3[0]);
													changed |= try_remove(d, d3[1]);
												}
												else
												{
													for (int i4 = 0; i4 < 9; ++i4)
													{
														for (int j4 = 0; j4 < 9; ++j4)
														{
															if ((i4 == i) && (j4 == j))
																continue;
															
															if ((i4 == i2) && (j4 == j2))
																continue;
															
															if ((i4 == i3) && (j4 == j3))
																continue;
															
															if (((i == i2) && (i2 == i3) && (i3 == i4)) ||
																((j == j2) && (j2 == j3) && (j3 == j4)) ||
																	((i / 3 == i2 / 3) && (i2 / 3 == i3 / 3) && (i3 / 3 == i4 / 3) &&
																	 (j / 3 == j2 / 3) && (j2 / 3 == j3 / 3) && (j3 / 3 == j4 / 3)))
															{
																vector<int>& d4 = v[i4][j4].domain;
																if ((2 <= d2.size()) && (d2.size() <= 3) &&
																	(2 <= d3.size()) && (d3.size() <= 3) &&
																	(2 <= d4.size()) && (d4.size() <= 3))
																{
																	set<int> digits;
																	digits.insert(d2.begin(), d2.end());
																	digits.insert(d3.begin(), d3.end());
																	digits.insert(d4.begin(), d4.end());
																	if (digits.size() <= 3)
																	{
																		for (int dig : digits)
																		{
																			changed |= try_remove(d, dig);
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
									break;
								}
							}
						}
					}
				}
			}
		} while (changed);
	}
	
	
	bool Search()
	{
		return Search(0, 0);
	}
	

	friend ostream& operator<<(ostream& os, const SudokuSolver& ss)
	{
		for (int i = 0; i < 9; ++i)
		{
			for (int j = 0; j < 9; ++j)
			{
				os << ss.v[i][j] << '\t';
			}
			os << endl;
		}
		return os;
	}
	

private:
	bool IsConsistent(int max_i, int max_j)
	{
		int x = v[max_i][max_j].x;
		
		for (int i = 0; i < max_i; ++i)
		{
			if (v[i][max_j].x == x)
			{
				return false;
			}
		}
		
		for (int j = 0; j < max_j; ++j)
		{
			if (v[max_i][j].x == x)
			{
				return false;
			}
		}

		for (int i = max_i / 3 * 3; i <= max_i; ++i)
		{
			for (int j = max_j / 3 * 3; j <= max_j; ++j)
			{
				if ((i != max_i) && (j != max_j))
				{
					if (v[i][j].x == x)
					{
						return false;
					}
				}
			}
		}
		
		return true;
	}
	
	
	bool Search(int i, int j)
	{
		if (i == 9)
		{
			return true;
		}
		
		Variable& var = v[i][j];
		vector<int>& domain = var.domain;
		if (domain.empty())
			return false;
			
		for (int x : var.domain)
		{
			var.x = x;
			if (!IsConsistent(i, j))
			{
				continue;
			}
			
			int next_i = (j == 8) ? i + 1 : i;
			int next_j = (j + 1) % 9;
			if (Search(next_i, next_j))
			{
				return true;
			}
		}
		
		return false;
	}
	
	
	static bool try_remove(vector<int>& d, int digit)
	{
		if (d.size() != 1)
		{
			vector<int>::iterator it = find(d.begin(), d.end(), digit);
			if (it != d.end())
			{
				d.erase(it);
				return true;
			}
		}
		
		return false;
	}
};


int main()
{
	auto start = chrono::system_clock::now();
	
	char board[9][10] =
	{
		"     6 73",
		"7 4  9   ",
		" 3  2    ",

		"  21    7",
		"  8 6 3  ",
		"5    84  ",

		"    9  5 ",
		"   3  1 8",
		"89 6     "
	};
	SudokuSolver ss(board);
		
	if (ss.Search())
	{
		cout << ss;
	}
	else
	{
		cout << "No solution" << endl;
	}	
	
	auto end = chrono::system_clock::now();
	auto elapsed = chrono::duration_cast<chrono::microseconds>(end - start).count();
	cout << elapsed << " us" << endl;
}