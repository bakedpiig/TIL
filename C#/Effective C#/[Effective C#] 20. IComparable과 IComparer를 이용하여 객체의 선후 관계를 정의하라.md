.NET Framework에서는 객체의 선후 관계를 정의하기 위해서 IComparable\<T>와 IComparer\<T>를 제공한다. IComparable\<T>는 기본적인 타입의 선후 관계를, IComparer\<T>는 추가적인 타입의 선후 관계를 정의한다.

IComparable에는 CompareTo() 하나의 메서드만 정의되어 있다. 현재 객체와 대상 객체보다 작으면 음수, 같으면 0, 크면 양수를 반환한다.

오래된 API들은 IComparable\<T>를 사용하지 않고 IComparable을 사용하기 때문에 둘이 같이 구현해야 할 때가 있다는 것을 알아둘 필요가 있다.

IComparable의 CompareTo()는 인자로 object 객체를 담는다. 따라서 이 메서드를 사용하면 런타임에 타입을 확인하게 된다. 즉 올바르지 않은 객체를 전달해도 컴파일타임에는 알 길이 없다. 그리고 박싱/언박싱 또한 많이 발생하기 때문에 비효율적이다.

~~~c#
public struct Customer: IComparable<Customer>, IComparable {
    private readonly string name;

    public Customer(string name) {
        this.name = name;
    }

    public int CompareTo(Customer other) => name.CompareTo(other.name);

    int IComparable.CompareTo(object obj) {
        if(!(obj is Customer)) 
            throw new ArgumentException("Argument is not a Customer","obj");
        
        Customer otherCustomer = (Customer)obj;
        return this.CompareTo(otherCustomer);
    }
}
~~~

IComparable 인터페이스에 있는 CompareTo를 쓰려면 객체를 명시적 캐스팅을 수행한 후 사용해야 한다. 그렇지 않으면 IComparable\<T>의 메서드를 호출하게 된다.

---
### IComparer
---
IComaparer는 정렬 기준을 정하는 데에 사용되는 인터페이스이다. 클래스 내에 이 인터페이스를 구현한 클래스를 정의해 놓으면 특정 기준을 통해서 정렬할 수 있다.

다음은 그 예시이다.

~~~c#
public struct Customer: IComparable<Customer>, IComparable {
    private readonly string name;
    private double revenue;

    public Customer(string name, double revenue) {
        this.name = name;
        this.revenue = revenue;
    }

    public int CompareTo(Customer other) => name.CompareTo(other.name);

    int IComparable.CompareTo(object obj) {
        if(!(obj is Customer))
            throw new ArgumentException("Argument is not a Customer","obj");
        
        Customer otherCustomer = (Customer)obj;

        return this.CompareTo(otherCustomer);
    }

    public static bool operator<(Customer left, Customer right) => left.CompareTo(right) < 0;
    public static bool operator<=(Customer left, Customer right) => left.CompareTo(right) <= 0;
    public static bool operator>(Customer left, Customer right) => left.CompareTo(right) > 0;
    public static bool operator>=(Customer left, Customer right) => left.CompareTo(right) >= 0;

    private static Lazy<RevenueComparer> revComp = new Lazy<RevenueComparer>(() => new RevenueComparer());

    public static IComparer<Customer> RevenueCompare => revComp.Value;

    public static Comparison<Customer> CompareByRevenue => (left, right) => left.revenue.ComapreTo(right.revenue);

    private class RevenueComparer: IComparer<Customer> {
        int IComparer<Customer>.Comapre(Customer left, Customer right) => left.revenue.CompareTo(right.revenue);
    }
}
~~~