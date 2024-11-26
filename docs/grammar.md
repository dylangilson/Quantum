$$
\begin{align}
    [\text{Program}] &\to [\text{Statement}]^*
    \\
    [\text{Statement}] &\to 
    \begin{cases}
        \text{exit}([\text{Expression}]);
        \\
        \text{[Declaration]}\space\text{identifier} = [\text{Expression}];
        \\
        \text{identifier} = \text{[Expression]};
        \\
        \text{if}\space([\text{Expression}])\space[\text{Scope}]\text{[IfPredicate]}
        \\
        [\text{Scope}]
    \end{cases}
    \\
    [\text{Expression}] &\to
    \begin{cases}
        [\text{Term}]
        \\
        [\text{BinaryExpression}]
    \end{cases}
    \\
    [\text{Term}] &\to
    \begin{cases}
        \text{integer\_literal}
        \\
        \text{identifier}
        \\
        ([\text{Expression}])
    \end{cases}
    \\
    [\text{BinaryExpression}] &\to
    \begin{cases}
        [\text{Expression}]\space*\space[\text{Expression}] & \text{precedence} = 1
        \\
        [\text{Expression}]\space\space/\space\space[\text{Expression}] & \text{precedence} = 1
        \\
        [\text{Expression}]\space+\space[\text{Expression}] & \text{precedence} = 0
        \\
        [\text{Expression}]\space-\space[\text{Expression}] & \text{precedence} = 0
    \end{cases}
    \\
    \text{[Declaration]} &\to
    \begin{cases}
        \text{let}
        \\
        \text{const}
    \end{cases}
    \\
    \text{[Scope]} &\to \{{[\text{Statement}]^*}\}
    \\
    \text{[IfPredicate]} &\to
    \begin{cases}
        \text{elif\text({[Expression]})\text{[Scope]}\text{[IfPredicate]}}
        \\
        \text{else}\text{[Scope]}
        \\
        \epsilon
    \end{cases}   
\end{align}
$$