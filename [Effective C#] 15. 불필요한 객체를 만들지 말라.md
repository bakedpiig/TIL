GC를 너무 믿고 불필요한 객체를 과도하게 만들어내면 GC도 과부하가 걸린다. 몇 가지 조항만 유념하면 GC의 작업을 줄일 수 있다.

---
### 자주 사용되는 지역변수는 멤버 변수로 변경한다
---
~~~c#
protected override void OnPaint(PaintEventArgs e) {
    using(Font font = new Font("Arial", 10.0f)) {
        e.Graphics.DrawString(DateTime.Now.ToString(),
            font, Brushes.Black, new PointF(0,0));
    }

    base.OnPaint(e);
}
~~~

위 코드는 윈도우의 Paint 이벤트 핸들러 중 많이 호출되는 핸들러이다. 위 메서드를 생성할 때마다 객체를 생성하고 빠른 시간 안에 제거한다는 것이 좋은 코드로 보일 수도 있지만, 애초에 이 과정에서 GC가 돌아가지 않게 하는 것이 좋다.

그래서 이 경우는 지역변수를 생성하는 것이 아니라 멤버 필드로 변경하여 재사용을 하게끔 한다.
~~~c#
Font font;

protected override void OnPaint(PaintEventArgs e) {
    e.Graphics.DrawString(DateTime.Now.ToString(),
        font, Brushes.Black, new PointF(0,0));

    base.OnPaint(e);
}
~~~

Font 타입과 같이 IDisposable 인터페이스를 구현한 타입의 객체를 멤버 변수로 사용하려면 이 클래스도 IDisposable을 구현해야 한다.

---
### 종속성 삽입을 활용한다
---
정말 자주 사용되는 변수라면 정적 변수로 사용하는 방법도 있다. 다음과 같이 코드를 짠다면 변수를 호출하지 않았을 때 메모리를 절약할 수 있다.

~~~c#
private static Brush blackBrush;

public static Brush Black {
    get {
        if(blackBrush == null)
            blackBrush = new SolidBrush(Color.Black);
        return blackBrush;
    }
}
~~~

다만 이런 방법은 생성된 객체가 메모리 상에 필요 이상으로 오랫동안 남아있을 수 있다. 또한 해제 시점을 모르기 때문에 비관리 리소스를 삭제할 수 없다는 것도 있다.

이 외에도 string 타입의 연산 시 발생하는 가비지 또한 있다. StringBuilder나 보간 문자열을 사용하면 연산 시에 사용되는 가비지를 줄일 수 있다.
