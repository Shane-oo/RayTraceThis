using MediatR;

namespace RayTraceThis.Net.Core.Exchange;

public interface ICommandHandler<in TCommand>: IRequestHandler<TCommand, Result> where TCommand : ICommand;

public interface ICommandHandler<in TCommand, TResponse>: IRequestHandler<TCommand, Result<TResponse>> where TCommand : ICommand<TResponse>;


public interface ICommand: IRequest<Result>, IRequestBase;

public interface ICommand<TResponse>: IRequest<Result<TResponse>>, IRequestBase;

public class Command<TResponse>: ICommand<TResponse>
{
}

public class Command: ICommand
{
}
