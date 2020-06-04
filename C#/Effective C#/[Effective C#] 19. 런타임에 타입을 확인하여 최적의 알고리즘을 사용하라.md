꼭 어느 순간에서나 제네릭을 사용하는 것이 좋은 것은 아니다. 구체적인 타입을 명시하여 생기는 장점이 크다면 굳이 제약 조건을 사용할 이유가 없다. 따라서 어떤 알고리즘이 특정 타입에 대해 더 효율적으로 동작한다고 하면 제약 조건을 설정하지 말고 그 타입만의 코드를 작성하는 것이 좋다.

제네릭의 인스턴스화는 런타임의 타입이 아닌 컴파일타임의 타입만을 고려한다.

~~~c#
public sealed class ReverseEnumerable<T>: IEnumerable<T> {
    private class ReverseEnumerator: IEnumerator<T> {
        int currentIndex;
        IList<T> collection;

        public ReverseEnumerator(IList<T> srcCollection) {
            collection = srcCollection;
            currentIndex = collection.Count;
        }

        public T Current => collection[currentIndex];

        public void Dispose() {
            //리소스 정리
        }

        object System.Collections.IEnumerator.Current => this.Current;
        public bool MoveNext() => --currentIndex >= 0;
        public void Reset() => currentIndex = collection.Count;
    }

    IEnumerable<T> sourceSequence;
    IList<T> originalSequence;

    public ReverseEnumerable(IEnumerable<T> sequence) {
        sourceSequence = sequence;
    }

    public IEnumerator<T> GetEnumerator() {
        if(originalSequence == null) {
            originalSequence = new List<T>();
            foreach(T item in sourceSequence)
                originalSequence.Add(item);
        }

        return new ReverseEnumerator(originalSequence);
    }

    System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() => this.GetEnumerator();
}
~~~

이를 각 타입에 알맞은 알고리즘을 적용시키도록 바꾸면 다음 코드와 같아진다.

~~~c#
public sealed class ReverseEnumerable<T>: IEnumerable<T> {
    private class ReverseEnumerator: IEnumerator<T> {
        int currentIndex;
        IList<T> collection;

        public ReverseEnumerator(IList<T> srcCollection) {
            collection = srcCollection;
            currentIndex = collection.Count;
        }

        public T Current => collection[currentIndex];

        public void Dispose() {
            //리소스 정리
        }

        object System.Collections.IEnumerator.Current => this.Current;
        public bool MoveNext() => --currentIndex >= 0;
        public void Reset() => currentIndex = collection.Count;
    }

    //string은 매우 특별한 경우이기 때문에 Enumerator도 따로 구현했다
    private sealed class ReverseStringEnumerator: IEnumerator<char> {
        private string sourceSequence;
        private int currentIndex;

        public ReverseStringEnumerator(string source) {
            sourceSequence = source;
            currentIndex = source.Length;
        }

        public char Current => sourceSequence[currentIndex];

        public void Dispose(){}

        object System.Collections.IEnumerator.Current => sourceSequence[currentIndex];
        public MoveNext() => --currentIndex >= 0;
        public void Reset() => currentIndex = sourceSequence.Length;
    }
    
    IEnumerable<T> sourceSequence;
    IList<T> originalSequence;

    public ReverseEnumerable(IEnumerable<T> sequence) {
        sourceSequence = sequence;
        originalSequence = sequence as IList<T>;
    }

    public ReverseEnumerable(IList<T> sequence) {
        sourceSequence = sequence;
        originalSequence = sequence;
    }

    public IEnumerator<T> GetEnumerator() {
        if(sourceSequence is string) {
            return new ReverseStringEnumerator(sourceSequence as string) as IEnumerator<T>;
        }

        if(originalSequence == null) {
            if(sourceSequence is ICollection<T>) {
                ICollection<T> source = sourceSequence as ICollection<T>;
                originalSequence = new List<T>(source.Count);
            }
            else
                originalSequence = new List<T>();

            foreach(T item in sourceSequence) 
                originalSequence.Add(item);
        }

        return new ReverseEnumerator(originalSequence);
    }

    System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() => this.GetEnumerator();
}
~~~