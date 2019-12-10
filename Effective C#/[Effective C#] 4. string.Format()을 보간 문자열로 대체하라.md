string.Format()의 단점은 눈으로의 코드 확인이 어려웠다는 것이다. 그에 비해 보간 문자열은 올바른 코드를 더 쉽게 작성할 수 있다.

보간 문자열은 문자열 앞에 '$'를 붙이면 된다. 문자열로 변경할 표현식은 { } 내에 둔다. 표현식에 제어문을 쓸 수는 없다.

사용자가 문자열 보간 기능을 사용해도 실제 컴파일러는 object 가변 배열을 전달하는 포매팅 함수를 호출한다. 이 과정에서 박싱이 수행되는데 이는 성능 면에서 좋지 않다. 따라서 전달할 인자를 사전에 문자열로 변경해서 보내는 것이 좋다. 필요하다면 포맷 형식을 위해 인자를 전달할 수도 있다.

~~~c#
Console.WriteLine($"The value of pi is {Math.PI.ToString("F2")}");

Console.WriteLine($"The value of pi is {Maht.PI:F2}");
~~~

위 두 문장은 같은 결과를 가져온다. 다음은 문자열 보간 기능을 사용한 예이다.

~~~c#
//삼항 연산자에서의 ':' 구분하는 방법
Console.WriteLine($"The value of pi is {(round ? Math.PI.ToString() : Math.PI.ToString("F2"
))}");


//?? 연산자 사용
Console.WriteLine($"The customer's name is {c?.Name ?? "Name is missing"}");

// 표현식 중첩 사용
string result = default(string);
Console.WriteLine($@"Record is {(records.TryGetValue(index, out result) ? result : $"No record found at index {index}")}");
~~~
