using MediatR;

namespace RayTraceThis.Net.Core.Exchange;

public interface IQueryHandler<in TQuery, TResponse>: IRequestHandler<TQuery, Result<TResponse>> where TQuery : IQuery<TResponse>;

public interface IQuery<TResponse>: IRequest<Result<TResponse>>, IRequestBase;

public class Query<TResponse>: IQuery<TResponse>
{
    #region Construction

    #endregion
}
