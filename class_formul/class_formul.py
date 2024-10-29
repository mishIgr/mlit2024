class Node:
    def __init__(self, value, left=None, right=None):
        self.value = value
        self.left = left
        self.right = right

# Определяем приоритеты операторов
__precedence = {
    '!': 5,  # Отрицание (унарный)
    '*': 4,  # Конъюнкция
    '|': 3,  # Дизъюнкция
    '>': 2,  # Импликация
    '+': 2,  # XOR
    '=': 1   # Эквиваленция
}

# Функция для создания дерева выражения
def __to_expression_tree(expression: str) -> Node:
    expression = expression.replace(" ", "")  # Убираем пробелы
    stack = []     # Стек для узлов
    operator_stack = []  # Стек для операторов

    i = 0
    while i < len(expression):
        token = expression[i]
        
        # Если символ - операнд, создаем узел и добавляем в стек
        if token.isalnum():
            stack.append(Node(token))
        
        # Если символ - оператор
        elif token in __precedence:
            # Обрабатываем приоритет операторов в стеке
            while (operator_stack and (operator_stack[-1] in __precedence) and
                   __precedence[operator_stack[-1]] >= __precedence[token]):
                op = operator_stack.pop()
                
                # Обработка унарного оператора (например, отрицания)
                if op == '!':
                    operand = stack.pop()
                    stack.append(Node(op, left=operand))
                else:
                    right = stack.pop()
                    left = stack.pop()
                    stack.append(Node(op, left, right))
            operator_stack.append(token)
        
        # Если открывающая скобка
        elif token == '(':
            operator_stack.append(token)
        
        # Если закрывающая скобка
        elif token == ')':
            while operator_stack and operator_stack[-1] != '(':
                op = operator_stack.pop()
                if op == '!':  # Унарный оператор
                    operand = stack.pop()
                    stack.append(Node(op, left=operand))
                else:  # Бинарные операторы
                    right = stack.pop()
                    left = stack.pop()
                    stack.append(Node(op, left, right))
            operator_stack.pop()  # Убираем '('
        
        else:
            raise ValueError('Неопознаный символ в формуле')
        
        i += 1

    # Выгружаем оставшиеся операторы из стека операторов
    while operator_stack:
        op = operator_stack.pop()
        if op == '!':  # Унарный оператор
            operand = stack.pop()
            stack.append(Node(op, left=operand))
        else:  # Бинарные операторы
            right = stack.pop()
            left = stack.pop()
            stack.append(Node(op, left, right))

    # Корень дерева - последний элемент стека
    return stack.pop()

class ErrorReadFormula(Exception):
    '''Ошибка в чтении формулы'''
    ...

class Formula:
    def __init__(self, formula: str) -> None:
        try:
            self.__tree = __to_expression_tree(formula)
        except ValueError:
            raise ErrorReadFormula(ErrorReadFormula.__doc__)
        except Exception as e:
            raise
    
    def __str__(self) -> str:
        if not self.__tree:
            return ''

        stack = [self.__tree]
        output = []
        while stack:
            node = stack.pop()
            output.append(str(node.value))  # Обработка узла
            
            # Сначала добавляем правого ребенка, затем левого
            if node.right:
                stack.append(node.right)
            if node.left:
                stack.append(node.left)
        return ' '.join(output)
    
    def formula_tree(self) -> str:
        q: list[Node] = [self.__tree]
        while q:
            print(' '.join(map(str, map(lambda x: x.value, q))))
            tmp_q = []
            for n in q:
                if n.left is not None:
                    tmp_q.append(n.left)
                if n.right is not None:
                    tmp_q.append(n.right)
            q = tmp_q
