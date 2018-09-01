#include<iostream>
using namespace std;

class Date
{
	public:
		Date(int year = 1900, int month = 1, int day = 1)//构造函数
			:_year(year)
			 , _month(month)
			 , _day(day)
	{
		if (!IsInvalid()) // this->IsInvalid(this) 
		{
			//assert(false);
			cout << "Is IsInvalid" << endl; 
      //exit(1);
		}
	}

		Date(const Date& d)//拷贝构造可以不写
		{
			_year = d._year;
			_month = d._month;
			_day = d._day;
		}
		Date& operator=(const Date& d)//赋值运算符的重载
		{
			if (*this != d)
			{
				_year = d._year;
				_month = d._month;
				_day = d._day;
			}
			return *this;
		}
		~Date()//析构函数可以不写
		{

		}
		bool IsInvalid()//判断日期是否合法
		{
			if (_month > 0 && _month < 13)
			{
				if (_day>0 && _day <= GetMonthDay(_year, _month))
					return true;
			}
			return false;
		}
		bool IsLeapYear(int year)//是否是闰年
		{
			if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
			{
				return true;
			}
			return false;

		}
		int GetMonthDay(int year, int month)//得到每月的天数
		{
			static int monthdays[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
			int day = monthdays[month];
			if (month == 2 && IsLeapYear(year))
			{
				day += 1;
			}
			return day;
		}

		void Show()//显示日期
		{
			cout << _year << "-" << _month << "-" << _day << endl;
		}

		bool operator==(const Date& d)//两个日期是否相等
		{
			return _year == d._year
				&&_month == d._month
				&&_day == d._day;
		}

		bool operator!=(const Date& d)//两个日期是否不相等
		{
			return !(*this == d);
		}

		bool operator>=(const Date& d)//>=的重载
		{
			return !(*this < d);
		}

		bool operator<=(const Date& d)//<=的重载
		{
			return (*this<d) || (*this == d);
		}

		bool operator>(const Date& d)//>的重载
		{
			return !(*this <= d);
		}		

		bool operator<(const Date& d)//<的重载
		{
			if ((_year < d._year)
					|| (_year == d._year&&_month < d._month)
					|| (_year == d._year&&_month == d._month&&_day < d._day)
				 )
			{
				return true;
			}
			return false;
		}

		Date operator+(int day)//日期加天数，不改变当前日期的值
		{
			Date ret(*this);
			ret._day += day;
			while (ret._day > GetMonthDay(ret._year, ret._month))
			{
				ret._day -= GetMonthDay(ret._year, ret._month);
				ret._month++;
				if (ret._month == 13)
				{
					ret._month = 1;
					++ret._year;
				}
			}
			return ret;
		}

		Date& operator+=(int day)//日期加天数，改变当前日期的值
		{
			*this = *this + day;
			return *this;
		}

		Date operator-(int day)//日期减天数，不改变当前日期的值
		{
			Date ret(*this);
			ret._day -= day;
			while(ret._day <= 0)
			{
				ret._month--;
				if (ret._month == 0)
				{
					ret._month = 12;
					ret._year--;
				}
				ret._day += GetMonthDay(ret._year, ret._month);
			}
			return ret;
		}

		Date& operator-=(int day)//日期减天数，改变当前日期的值
		{
			*this = *this - day;
			return *this;
		}

		int operator-(const Date& d)//日期相减
		{
			Date Max(*this);
			Date Min(d);
			int flag = 1;
			if (*this < d)
			{
				Max = d;
				Min = *this;
				flag = -1;
			}
			int days = 0;
			while (Max != Min)
			{
				--Max;
				++days;
			}
			return days*flag;
		}

		//++d1 
		Date& operator++() // 前置++ 
		{
			*this = *this + 1;
			return *this;
		}
		//d1++ 
		Date operator++(int) // 后置++ 
		{
			Date ret(*this);
			*this = *this + 1;
			return ret;
		}

		Date operator--()//前置--
		{
			*this = *this - 1;
			return *this;
		}

		Date operator--(int)//后置--
		{
			Date ret(*this);
			*this = *this - 1;
			return ret;
		}

		friend ostream& operator<<(ostream& os, const Date& d);    //输出  

		friend istream& operator>>(istream& is, Date& d);         //输入  
	private:
		int _year;
		int _month;
		int _day;
};
ostream& operator<<(ostream& os, const Date& d)
{
	os << d._year << "-" << d._month << "-" << d._day ;
	//os ;  
	//os ;  
	return os;
}

istream& operator>>(istream& is, Date& d)
{
	cout << "输入日期：";
	is >> d._year;
	is >> d._month;
	is >> d._day;

	return is;
}
